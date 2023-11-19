#include <cmath>

#include "sn-vcv-lfo.hpp"
#include "sn-vcv-lfox.hpp"
#include "sn-vcv.hpp"

// clang-format off
const range RANGES[] = {
    { .index = 0, .defval = 5.f,    .base = 1.5848933f, .multiplier = 0.1f,  .offset = 0.f,  .precision = 2 },
    { .index = 1, .defval = 5.f,    .base = 1.5848933f, .multiplier = 0.02f, .offset = 0.f,  .precision = 3 },
    { .index = 2, .defval = 1.837f, .base = 0.f,        .multiplier = 2.45f, .offset = 0.5f, .precision = 2 },
};
// clang-format on

const int sn_vcv_lfo::CHANNELS = 1;

sn_vcv_lfo::sn_vcv_lfo() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // clang-format off
    configParam(FREQUENCY_PARAM,
                0.f,
                10.f,
                range.defval,
                "frequency",
                "Hz",
                range.base,
                range.multiplier,
                range.offset)->displayPrecision = range.precision;
    // clang-format on

    configParam(ECCENTRICITY_PARAM, -1.0f, +1.0f, 0.0f, "ε");
    configParam(SENSITIVITY_PARAM, 0.0f, +5.0f, 2.5f, "s");
    configParam(ROTATION_PARAM, -90.0f, +90.0f, 0.0f, "θ");
    configParam(AMPLITUDE_PARAM, 0.0f, +1.0f, 1.0f, "a");
    configParam(DX_PARAM, -1.0f, +1.0f, 0.0f, "δx");
    configParam(DY_PARAM, -1.0f, +1.0f, 0.0f, "δy");
    configParam(PHI_PARAM, -90.0f, +90.0f, 0.0f, "Φ");
    configSwitch(M_PARAM, 1.f, 5.f, 1.f, "m", {"1", "2", "3", "4", "5"});

    getParamQuantity(M_PARAM)->randomizeEnabled = false;

    configInput(FREQUENCY_INPUT, "0-10V, log frequency (0.1 to 10Hz)");
    configInput(SYNCH_INPUT, "Trigger input to sync phase");

    configOutput(LFO_OUTPUT, "LFO");
    configOutput(AUX_OUTPUT, "AUX");
    configOutput(AUX_TRIGGER, "Trigger");

    // ... reset trigger
    trigger.reset();
}

json_t *sn_vcv_lfo::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "range", json_integer(getRange()));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));

    return root;
}

void sn_vcv_lfo::dataFromJson(json_t *root) {
    json_t *krate = json_object_get(root, "k-rate");
    json_t *range = json_object_get(root, "range");
    json_t *aux_mode = json_object_get(root, "aux-mode");

    if (krate) {
        int v = json_integer_value(krate);

        if (v >= 0 && v < 4) {
            update.krate = json_integer_value(krate);
        }
    }

    if (range) {
        int v = json_integer_value(range);

        if (v >= 0 && v <= 2) {
            setRange(json_integer_value(range));
        }
    }

    if (aux_mode) {
        int v = json_integer_value(aux_mode);

        if (v == 0) {
            aux.mode = OSC;
        } else if (v == 1) {
            aux.mode = SUM;
        } else if (v == 2)
            aux.mode = POLY;
    }
}

void sn_vcv_lfo::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    if (left && left->model == modelSn_vcv_lfox) {
        expanders.left = left;
    } else {
        expanders.left = NULL;
    }

    if (right && right->model == modelSn_vcv_lfox) {
        expanders.right = right;
    } else {
        expanders.right = NULL;
    }
}

void sn_vcv_lfo::process(const ProcessArgs &args) {
    int channels = this->channels();
    bool resync = sync.process(inputs[SYNCH_INPUT].getVoltage());

    // ... expander
    bool expanded = expanders.left != NULL || expanders.right != NULL;
    bool xll = false;
    bool xrr = expanders.right;

    if (expanders.left) {
        sn_vcv_lfox *x = (sn_vcv_lfox *)expanders.left;

        if (!x->isLinkedLeft()) {
            xll = true;
        }
    }

    lights[XLL_LIGHT].setBrightnessSmooth(xll ? 1.0 : 0.f, args.sampleTime);
    lights[XRR_LIGHT].setBrightnessSmooth(xrr ? 1.0 : 0.f, args.sampleTime);

    // ... get params and recompute transform matrix
    bool recalculate = false;

    update.count--;

    if (update.count <= 0) {
        recompute();
        recalculate = true;
        update.count = KRATE[update.krate];
    }

    // ... generate
    processLFO(args, channels, resync, expanded, recalculate);
    processAUX(args, expanded, resync);

    // ... update expanders
    if (expanders.left) {
        sn_lfo_message *msg = (sn_lfo_message *)expanders.left->getRightExpander().producerMessage;

        if (msg != NULL) {
            msg->linked = true;
            msg->channels = channels;

            for (int ch = 0; ch < channels; ch++) {
                msg->LFO[ch].phase = LFO.phase[ch];
                msg->LFO[ch].out = LFO.out[ch];
            }

            msg->AUX.phase = aux.phase;
            msg->AUX.out = aux.out.sum;

            expanders.left->getRightExpander().requestMessageFlip();
        }
    }

    if (expanders.right) {
        sn_lfo_message *msg = (sn_lfo_message *)expanders.right->getLeftExpander().producerMessage;

        if (msg != NULL) {
            msg->linked = true;
            msg->channels = channels;

            for (int ch = 0; ch < channels; ch++) {
                msg->LFO[ch].phase = LFO.phase[ch];
                msg->LFO[ch].out = LFO.out[ch];
            }

            msg->AUX.phase = aux.phase;
            msg->AUX.out = aux.out.sum;

            expanders.right->getLeftExpander().requestMessageFlip();
        }
    }
}

void sn_vcv_lfo::recompute() {
    // ... param values
    float e = params[ECCENTRICITY_PARAM].getValue();
    float s = params[SENSITIVITY_PARAM].getValue();
    float θ = params[ROTATION_PARAM].getValue();
    float A = params[AMPLITUDE_PARAM].getValue();
    float δx = params[DX_PARAM].getValue();
    float δy = params[DY_PARAM].getValue();
    float Φ = params[PHI_PARAM].getValue();
    float m = params[M_PARAM].getValue();

    // ... set internal SN parameters
    sn.ε = std::tanh(s * e);
    sn.θ = clamp(θ, -89.95f, +89.95f) * M_PI / 180.0f;
    sn.A = A;
    sn.δx = δx;
    sn.δy = δy;
    sn.Φ = Φ;
    sn.m = m;

    sn.recompute();
}

void sn_vcv_lfo::processLFO(const ProcessArgs &args, int channels, bool resync, bool expanded, bool recalculate) {
    // ... update instantaneous phase
    float fv = params[FREQUENCY_PARAM].getValue();
    float f = range.frequency(fv);
    float frequency[16] = {f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

    if (inputs[FREQUENCY_INPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            frequency[ch] = clamp(inputs[FREQUENCY_INPUT].getPolyVoltage(ch), 0.1f, 10.0f);
        }
    }

    for (int ch = 0; ch < 16; ch++) {
        if (ch < channels) {
            LFO.phase[ch] += frequency[ch] * args.sampleTime;
            if (LFO.phase[ch] >= 1.f) {
                LFO.phase[ch] -= 1.f;
            }
        } else {
            LFO.phase[ch] = 0.f;
        }
    }

    if (resync) {
        for (int ch = 0; ch < 16; ch++) {
            LFO.phase[ch] = 0.f;
        }
    }

    // ... generate LFO signal
    if ((outputs[LFO_OUTPUT].isConnected() || expanded) && recalculate) {
        for (int ch = 0; ch < channels; ch++) {
            float α = 2.0f * M_PI * LFO.phase[ch];
            float υ = sn.υ(α);

            LFO.out[ch] = sn.A * υ;
        }
    }

    if (outputs[LFO_OUTPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[LFO_OUTPUT].setVoltage(5.f * LFO.out[ch], ch);
        }

        outputs[LFO_OUTPUT].setChannels(channels);
    }
}

void sn_vcv_lfo::processAUX(const ProcessArgs &args, bool expanded, bool resync) {
    // ... update instantaneous phase
    aux.phase += AUX_FREQUENCY * args.sampleTime;
    if (aux.phase >= 1.f) {
        aux.phase -= 1.f;

        if (outputs[AUX_TRIGGER].isConnected()) {
            trigger.trigger(0.001f);
        }
    }

    if (resync) {
        aux.phase = 0.f;
    }

    bool triggered = trigger.process(args.sampleTime);

    // ... generate AUX signal
    // NTS: aux is recalculated on every sample because f is 25Hz and artifacts are visible
    if (outputs[AUX_OUTPUT].isConnected() || expanded) {
        float α = aux.phase * 2.0f * M_PI;
        float υ = sn.υ(α);

        aux.out.osc = υ;
        aux.out.sum = sn.A * υ;
    } else {
        aux.out.osc = 0.0f;
        aux.out.sum = 0.0f;
    }

    // ... set outputs
    if (outputs[AUX_TRIGGER].isConnected()) {
        outputs[AUX_TRIGGER].setVoltage(triggered ? 10.f : 0.0f);
    }

    if (outputs[AUX_OUTPUT].isConnected()) {
        switch (aux.mode) {
        case POLY:
            outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.osc, 0);
            outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.sum, 1);
            outputs[AUX_OUTPUT].setChannels(2);
            break;

        case SUM:
            outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.sum);
            outputs[AUX_OUTPUT].setChannels(1);
            break;

        default:
            outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.osc);
            outputs[AUX_OUTPUT].setChannels(1);
        }
    }
}

int sn_vcv_lfo::channels() {
    return inputs[FREQUENCY_INPUT].isConnected() ? inputs[FREQUENCY_INPUT].getChannels() : CHANNELS;
}

int sn_vcv_lfo::getRange() {
    return range.index;
}

void sn_vcv_lfo::setRange(int v) {
    ParamQuantity *param = getParamQuantity(FREQUENCY_PARAM);

    if (v >= 0 && v <= 2) {
        range = RANGES[v];

        param->displayBase = range.base;
        param->displayMultiplier = range.multiplier;
        param->displayOffset = range.offset;
        param->displayPrecision = range.precision;
        param->defaultValue = range.defval;
    }
}

sn_vcv_lfoWidget::sn_vcv_lfoWidget(sn_vcv_lfo *module) {
    float left = 8.89;
    float right = 27.94;
    float top = 21.968 + 1.27;
    float dh = 13.014;

    Vec e(left, top);
    Vec s(left, top + dh);
    Vec θ(left, top + 2 * dh);
    Vec A(left, top + 3 * dh);
    Vec δx(left, top + 4 * dh);
    Vec δy(left, top + 5 * dh);
    Vec 𝜓(left, top + 6 * dh);
    Vec m(left, top + 7 * dh);

    Vec f(26.67, 27.94 - 0.5 + 1.27);
    Vec fin(26.67, 40.64 - 0.5 + 1.27);
    Vec synch(26.67, 53.34 + 1.27);

    Vec aux(right, top + 4 * dh);
    Vec trigger(right, top + 5 * dh);
    Vec lfo(right, top + 7 * dh);

    Vec xll(2.54, 13.97);
    Vec xrr(33.02, 13.97);

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-vcv-lfo.svg"),
                         asset::plugin(pluginInstance, "res/sn-vcv-lfo-dark.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... parameters
    addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(f), module, sn_vcv_lfo::FREQUENCY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(e), module, sn_vcv_lfo::ECCENTRICITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(s), module, sn_vcv_lfo::SENSITIVITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(θ), module, sn_vcv_lfo::ROTATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(A), module, sn_vcv_lfo::AMPLITUDE_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(δx), module, sn_vcv_lfo::DX_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(δy), module, sn_vcv_lfo::DY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(𝜓), module, sn_vcv_lfo::PHI_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(m), module, sn_vcv_lfo::M_PARAM));

    // ... inputs
    addInput(createInputCentered<PJ301MPort>(mm2px(fin), module, sn_vcv_lfo::FREQUENCY_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(synch), module, sn_vcv_lfo::SYNCH_INPUT));

    // ... outputs
    addOutput(createOutputCentered<PJ301MPort>(mm2px(lfo), module, sn_vcv_lfo::LFO_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(aux), module, sn_vcv_lfo::AUX_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(trigger), module, sn_vcv_lfo::AUX_TRIGGER));

    // ... expander indicators
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_vcv_lfo::XLL_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_vcv_lfo::XRR_LIGHT));
}

void sn_vcv_lfoWidget::appendContextMenu(Menu *menu) {
    sn_vcv_lfo *module = getModule<sn_vcv_lfo>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-lfo settings"));

    menu->addChild(createIndexPtrSubmenuItem("k-rate",
                                             KRATES,
                                             &module->update.krate));

    menu->addChild(createIndexPtrSubmenuItem("aux-mode",
                                             AUX_MODES,
                                             &module->aux.mode));

    menu->addChild(createIndexSubmenuItem(
        "range",
        {"0.1-10Hz", "0.02-2Hz", "0.5-25Hz"},
        [=]() {
            return module->getRange();
        },
        [=](int range) {
            module->setRange(range);
        }));
}

Model *modelSn_vcv_lfo = createModel<sn_vcv_lfo, sn_vcv_lfoWidget>("sn-vcv-lfo");