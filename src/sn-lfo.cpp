#include <cmath>

#include "sn-lfo.hpp"
#include "sn-lfox.hpp"
#include "sn.hpp"

// clang-format off
const range RANGES[] = {
    { .index = 0, .defval = 5.f,    .base = 1.5848933f, .multiplier = 0.1f,  .offset = 0.f,  .precision = 2 },
    { .index = 1, .defval = 5.f,    .base = 1.5848933f, .multiplier = 0.02f, .offset = 0.f,  .precision = 3 },
    { .index = 2, .defval = 1.837f, .base = 0.f,        .multiplier = 2.45f, .offset = 0.5f, .precision = 2 },
};
// clang-format on

const int sn_lfo::CHANNELS = 1;

sn_lfo::sn_lfo() {
    // ... params
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    configParam(ECCENTRICITY_PARAM, -1.0f, +1.0f, 0.0f, "ε");
    configParam(SENSITIVITY_PARAM, 0.0f, +5.0f, 2.5f, "s");
    configParam(ROTATION_PARAM, -90.0f, +90.0f, 0.0f, "θ");
    configParam(AMPLITUDE_PARAM, 0.0f, +1.0f, 1.0f, "a");
    configParam(DX_PARAM, -1.0f, +1.0f, 0.0f, "δx");
    configParam(DY_PARAM, -1.0f, +1.0f, 0.0f, "δy");
    configParam(PHI_PARAM, -90.0f, +90.0f, 0.0f, "Φ");
    configSwitch(M_PARAM, 1.f, 5.f, 1.f, "m", {"1", "2", "3", "4", "5"});

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

    getParamQuantity(M_PARAM)->randomizeEnabled = false;

    // ... inputs
    configInput(FREQUENCY_INPUT, "±5V, log frequency modulation");
    configInput(SYNCH_INPUT, "Trigger input to sync phase");

    // ... outputs
    configOutput(LFO_OUTPUT, "LFO");
    configOutput(AUX_OUTPUT, "AUX");
    configOutput(AUX_TRIGGER, "Trigger");

    // ... reset trigger
    trigger.reset();
}

json_t *sn_lfo::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "range", json_integer(getRange()));
    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));
    json_object_set_new(root, "aux-gain", json_integer(aux.gain));

    return root;
}

void sn_lfo::dataFromJson(json_t *root) {
    json_t *range = json_object_get(root, "range");
    json_t *krate = json_object_get(root, "k-rate");
    json_t *aux_mode = json_object_get(root, "aux-mode");
    json_t *gain = json_object_get(root, "aux-gain");

    if (range) {
        int v = json_integer_value(range);

        if (v >= 0 && v <= 2) {
            setRange(json_integer_value(range));
        }
    }

    if (krate) {
        int v = json_integer_value(krate);

        if (v >= 0 && v < 4) {
            update.krate = json_integer_value(krate);
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

    if (gain) {
        int v = json_integer_value(gain);

        if (v >= 0 && v <= 4) {
            aux.gain = v;
        }
    }
}

void sn_lfo::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    if (left && left->model == model_sn_lfox) {
        expanders.left.module = left;
    } else {
        expanders.left.module = NULL;
    }

    if (right && right->model == model_sn_lfox) {
        expanders.right.module = right;
    } else {
        expanders.right.module = NULL;
    }
}

void sn_lfo::process(const ProcessArgs &args) {
    int channels = this->channels();
    bool resync = sync.process(inputs[SYNCH_INPUT].getVoltage());

    // ... expander
    bool expanded = expanders.left.module != NULL || expanders.right.module != NULL;
    bool xll = false;
    bool xrr = expanders.right.module != NULL;

    if (expanders.left.module != NULL) {
        sn_lfox *x = (sn_lfox *)expanders.left.module;

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
    {
        sn_lfo_message *msg;

        if ((msg = expanders.left.producer()) != NULL) {
            msg->set(true, channels, lfo, aux);
            expanders.left.flip();
        }

        if ((msg = expanders.right.producer()) != NULL) {
            msg->set(true, channels, lfo, aux);
            expanders.right.flip();
        }
    }
}

void sn_lfo::recompute() {
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
    sn.Φ = clamp(Φ, -89.95f, +89.95f) * M_PI / 180.0f;
    sn.m = m;

    sn.recompute();
}

void sn_lfo::processLFO(const ProcessArgs &args, int channels, bool resync, bool expanded, bool recalculate) {
    // ... update instantaneous phase
    float fv = params[FREQUENCY_PARAM].getValue();
    float f = range.frequency(fv);
    float frequency[16] = {f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f};

    if (resync) {
        for (int ch = 0; ch < 16; ch++) {
            lfo[ch].phase = 0.f;
        }
    }

    if (inputs[FREQUENCY_INPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            float fm = 10.f * clamp(inputs[FREQUENCY_INPUT].getPolyVoltage(ch) / 5.0f, -1.0f, +1.0f);
            float v = clamp(fv + fm, 0.f, 10.f);

            frequency[ch] = range.frequency(v);
        }
    }

    for (int ch = 0; ch < 16; ch++) {
        if (ch < channels) {
            lfo[ch].phase += frequency[ch] * args.sampleTime;
            if (lfo[ch].phase >= 1.f) {
                lfo[ch].phase -= 1.f;
            }
        } else {
            lfo[ch].phase = 0.f;
        }
    }

    // ... generate LFO signal
    if (outputs[LFO_OUTPUT].isConnected() || expanded || recalculate) {
        for (int ch = 0; ch < channels; ch++) {
            float α = 2.0f * M_PI * lfo[ch].phase;
            float υ = sn.υ(α);

            lfo[ch].out.lfo = υ;
            lfo[ch].out.sum = sn.A * υ;
        }
    }

    if (outputs[LFO_OUTPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[LFO_OUTPUT].setVoltage(5.f * lfo[ch].out.lfo, ch);
        }

        outputs[LFO_OUTPUT].setChannels(channels);
    }
}

void sn_lfo::processAUX(const ProcessArgs &args, bool expanded, bool resync) {
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

        aux.out.osc = sn.A * υ;
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
        float g = AUX_GAIN[aux.gain];

        switch (aux.mode) {
        case POLY:
            outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.osc, 0);
            outputs[AUX_OUTPUT].setVoltage(5.f * g * aux.out.sum, 1);
            outputs[AUX_OUTPUT].setChannels(2);
            break;

        case SUM:
            outputs[AUX_OUTPUT].setVoltage(5.f * g * aux.out.sum);
            outputs[AUX_OUTPUT].setChannels(1);
            break;

        default:
            outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.osc);
            outputs[AUX_OUTPUT].setChannels(1);
        }
    }
}

int sn_lfo::channels() {
    return inputs[FREQUENCY_INPUT].isConnected() ? inputs[FREQUENCY_INPUT].getChannels() : CHANNELS;
}

int sn_lfo::getRange() {
    return range.index;
}

void sn_lfo::setRange(int v) {
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

sn_lfoWidget::sn_lfoWidget(sn_lfo *module) {
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
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-lfo.svg"),
                         asset::plugin(pluginInstance, "res/sn-lfo-dark.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... parameters
    addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(f), module, sn_lfo::FREQUENCY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(e), module, sn_lfo::ECCENTRICITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(s), module, sn_lfo::SENSITIVITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(θ), module, sn_lfo::ROTATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(A), module, sn_lfo::AMPLITUDE_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(δx), module, sn_lfo::DX_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(δy), module, sn_lfo::DY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(𝜓), module, sn_lfo::PHI_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(m), module, sn_lfo::M_PARAM));

    // ... inputs
    addInput(createInputCentered<PJ301MPort>(mm2px(fin), module, sn_lfo::FREQUENCY_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(synch), module, sn_lfo::SYNCH_INPUT));

    // ... outputs
    addOutput(createOutputCentered<PJ301MPort>(mm2px(lfo), module, sn_lfo::LFO_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(aux), module, sn_lfo::AUX_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(trigger), module, sn_lfo::AUX_TRIGGER));

    // ... expander indicators
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_lfo::XLL_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_lfo::XRR_LIGHT));
}

void sn_lfoWidget::appendContextMenu(Menu *menu) {
    sn_lfo *module = getModule<sn_lfo>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-lfo settings"));

    menu->addChild(createIndexSubmenuItem(
        "range",
        {"0.1-10Hz", "0.02-2Hz", "0.5-25Hz"},
        [=]() {
            return module->getRange();
        },
        [=](int range) {
            module->setRange(range);
        }));

    menu->addChild(createIndexPtrSubmenuItem("k-rate",
                                             KRATES,
                                             &module->update.krate));

    menu->addChild(createIndexPtrSubmenuItem("aux-mode",
                                             AUX_MODES,
                                             &module->aux.mode));

    menu->addChild(createIndexPtrSubmenuItem("aux-gain",
                                             AUX_GAINS,
                                             &module->aux.gain));
}

Model *model_sn_lfo = createModel<sn_lfo, sn_lfoWidget>("sn-lfo");
