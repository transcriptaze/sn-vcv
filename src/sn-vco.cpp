#include "sn-vco.hpp"
#include "sn-vcox.hpp"

const int sn_vco::CHANNELS = 1;
const float sn_vco::VELOCITY = 1.0f;

sn_vco::sn_vco() {
    // ... params
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    configParam(ECCENTRICITY_PARAM, -1.0f, +1.0f, 0.0f, "ε");
    configParam(SENSITIVITY_PARAM, 0.0f, +5.0f, 2.5f, "s");
    configParam(ROTATION_PARAM, -90.0f, +90.0f, 0.0f, "θ");
    configParam(AMPLITUDE_PARAM, 0.0f, +1.0f, 1.0f, "a");
    configParam(DX_PARAM, -1.0f, +1.0f, 0.0f, "δx");
    configParam(DY_PARAM, -1.0f, +1.0f, 0.0f, "δy");
    configSwitch(M_PARAM, 1.f, 5.f, 1.f, "m", {"1", "2", "3", "4", "5"});

    getParamQuantity(PLACEHOLDER_PARAM)->randomizeEnabled = false;
    getParamQuantity(M_PARAM)->randomizeEnabled = false;

    // ... inputs
    configInput(ECCENTRICITY_INPUT, "±5V ε");
    configInput(SENSITIVITY_INPUT, "±5V s");
    configInput(ROTATION_INPUT, "±5V Φ");
    configInput(AMPLITUDE_INPUT, "±5V a");
    configInput(DX_INPUT, "±5V δx");
    configInput(DY_INPUT, "±5V δy");

    configInput(PITCH_INPUT, "1V/octave pitch");
    configInput(VELOCITY_INPUT, "0-10V velocity");

    // ... outputs
    configOutput(VCO_OUTPUT, "VCO");
    configOutput(AUX_OUTPUT, "AUX");
    configOutput(AUX_TRIGGER, "Trigger");

    // ... reset trigger
    trigger.reset();
}

json_t *sn_vco::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));
    json_object_set_new(root, "aux-gain", json_integer(aux.gain));

    return root;
}

void sn_vco::dataFromJson(json_t *root) {
    json_t *krate = json_object_get(root, "k-rate");
    json_t *aux_mode = json_object_get(root, "aux-mode");
    json_t *gain = json_object_get(root, "aux-gain");

    if (krate) {
        int v = json_integer_value(krate);

        if (v >= 0 && v < 4) {
            update.krate = v;
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

void sn_vco::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    if (left && left->model == model_sn_vcox) {
        expanders.left.module = left;
    } else {
        expanders.left.module = NULL;
    }

    if (right && right->model == model_sn_vcox) {
        expanders.right.module = right;
    } else {
        expanders.right.module = NULL;
    }
}

void sn_vco::process(const ProcessArgs &args) {
    int channels = this->channels();

    // ... expanders
    bool expanded = expanders.left.module != NULL || expanders.right.module != NULL;
    bool xll = false;
    bool xrr = expanders.right.module != NULL;

    if (expanders.left.module != NULL) {
        sn_vcox *x = (sn_vcox *)expanders.left.module;
        if (!x->isLinkedLeft()) {
            xll = true;
        }
    }

    lights[XLL_LIGHT].setBrightnessSmooth(xll ? 1.0 : 0.f, args.sampleTime);
    lights[XRR_LIGHT].setBrightnessSmooth(xrr ? 1.0 : 0.f, args.sampleTime);

    // ... get params and recompute transform matrix
    update.count--;

    if (update.count <= 0) {
        recompute();
        update.count = KRATE[update.krate];
    }

    // ... generate
    processVCO(args, channels, expanded);
    processAUX(args, expanded);

    // ... update expanders
    {
        sn_vco_message *msg;

        if ((msg = expanders.left.producer()) != NULL) {
            msg->set(true, channels, vco, aux);
            expanders.left.flip();
        }

        if ((msg = expanders.right.producer()) != NULL) {
            msg->set(true, channels, vco, aux);
            expanders.right.flip();
        }
    }
}

void sn_vco::processVCO(const ProcessArgs &args, int channels, bool expanded) {
    bool connected = outputs[VCO_OUTPUT].isConnected();

    // ... convert pitch CV to instantaneous frequency
    for (int ch = 0; ch < channels; ch++) {
        float pitch = inputs[PITCH_INPUT].getPolyVoltage(ch);
        float f = dsp::FREQ_C4 * std::pow(2.f, pitch);

        vco[ch].phase += f * args.sampleTime;
        while (vco[ch].phase >= 1.f) {
            vco[ch].phase -= 1.f;
        }
    }

    // ... generate
    if (connected || expanded) {
        for (int ch = 0; ch < channels; ch++) {
            float α = vco[ch].phase * 2.0f * M_PI;
            float υ = sn.υ(α);

            vco[ch].out.vco = υ;
            vco[ch].out.sum = sn.A * υ;
            vco[ch].velocity = velocity(ch);
        }
    }

    if (connected) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[VCO_OUTPUT].setVoltage(5.f * vco[ch].velocity * vco[ch].out.vco, ch);
        }

        outputs[VCO_OUTPUT].setChannels(channels);
    }
}

void sn_vco::processAUX(const ProcessArgs &args, bool expanded) {
    aux.phase += AUX_FREQUENCY * args.sampleTime;
    while (aux.phase >= 1.f) {
        aux.phase -= 1.f;

        if (outputs[AUX_TRIGGER].isConnected()) {
            trigger.trigger(0.001f);
        }
    }

    bool triggered = trigger.process(args.sampleTime);

    if (outputs[AUX_OUTPUT].isConnected() || expanded) {
        float α = aux.phase * 2.0f * M_PI;
        float υ = sn.υ(α);

        aux.out.osc = sn.A * υ;
        aux.out.sum = sn.A * υ;
    } else {
        aux.out.osc = 0.0f;
        aux.out.sum = 0.0f;
    }

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

void sn_vco::recompute() {
    // ... param values
    float e = params[ECCENTRICITY_PARAM].getValue();
    float s = params[SENSITIVITY_PARAM].getValue();
    float θ = params[ROTATION_PARAM].getValue();
    float A = params[AMPLITUDE_PARAM].getValue();
    float δx = params[DX_PARAM].getValue();
    float δy = params[DY_PARAM].getValue();
    float m = params[M_PARAM].getValue();

    // ... modulate with inputs
    e += clamp(inputs[ECCENTRICITY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    s += 5.0f * clamp(inputs[SENSITIVITY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    θ += 90.0f * clamp(inputs[ROTATION_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    A += clamp(inputs[AMPLITUDE_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    δx += clamp(inputs[DX_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    δy += clamp(inputs[DY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);

    // ... preconditioning
    e = clamp(e, -1.0f, +1.0f);
    s = clamp(s, 0.0f, +5.0f);
    θ = clamp(θ, -89.95f, +89.95f);

    // ... recalculate SN parameters
    sn.ε = std::tanh(s * e);
    sn.θ = θ * M_PI / 180.0f;
    sn.A = clamp(A, 0.0f, +1.0f);
    sn.δx = clamp(δx, -1.0f, +1.0f);
    sn.δy = clamp(δy, -1.0f, +1.0f);
    sn.m = m;

    sn.recompute();
}

int sn_vco::channels() {
    return inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;
}

float sn_vco::velocity(int channel) {
    if (inputs[VELOCITY_INPUT].isConnected()) {
        int N = inputs[VELOCITY_INPUT].getChannels();

        if (channel < N) {
            return inputs[VELOCITY_INPUT].getPolyVoltage(channel) / 10.0f;
        } else {
            return inputs[VELOCITY_INPUT].getVoltage() / 10.0f;
        }
    }

    return VELOCITY;
}

sn_vcoWidget::sn_vcoWidget(sn_vco *module) {
    float left = 7.331;
    float middle = 20.351;
    float right = 35.56;
    float top = 21.968 + 1.27;
    float dh = 13.014;

    Vec input_e(left, top);
    Vec input_s(left, top + dh);
    Vec input_θ(left, top + 2 * dh);
    Vec input_A(left, top + 3 * dh);
    Vec input_δx(left, top + 4 * dh);
    Vec input_δy(left, top + 5 * dh);

    Vec param_e(middle, top);
    Vec param_s(middle, top + dh);
    Vec param_θ(middle, top + 2 * dh);
    Vec param_A(middle, top + 3 * dh);
    Vec param_δx(middle, top + 4 * dh);
    Vec param_δy(middle, top + 5 * dh);
    Vec param_m(middle, top + 6 * dh);

    Vec channels(right - 3.81, top - 3.81);
    Vec pitch(right, top + dh);
    Vec velocity(right, top + 2 * dh);

    Vec aux(right, top + 4 * dh);
    Vec trigger(right, top + 5 * dh);
    Vec vco(right, top + 7 * dh);

    Vec xll(2.54, 11.43 + 2.54);
    Vec xrr(43.18, 11.43 + 2.54);

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-vco.svg"),
                         asset::plugin(pluginInstance, "res/sn-vco-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... e
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_e), module, sn_vco::ECCENTRICITY_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_e), module, sn_vco::ECCENTRICITY_PARAM));

    // ... s
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_s), module, sn_vco::SENSITIVITY_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_s), module, sn_vco::SENSITIVITY_PARAM));

    // ... θ
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_θ), module, sn_vco::ROTATION_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_θ), module, sn_vco::ROTATION_PARAM));

    // ... A
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_A), module, sn_vco::AMPLITUDE_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_A), module, sn_vco::AMPLITUDE_PARAM));

    // ... δx
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δx), module, sn_vco::DX_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δx), module, sn_vco::DX_PARAM));

    // ... δy
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δy), module, sn_vco::DY_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δy), module, sn_vco::DY_PARAM));

    // ... m,
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_m), module, sn_vco::M_PARAM));

    // ... pitch and velocity inputs
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(pitch), module, sn_vco::PITCH_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(velocity), module, sn_vco::VELOCITY_INPUT));

    // ... aux, trigger and VCO outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(aux), module, sn_vco::AUX_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(trigger), module, sn_vco::AUX_TRIGGER));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(vco), module, sn_vco::VCO_OUTPUT));

    // ... channels
    sn_vco_channels *lcd = createWidget<sn_vco_channels>(mm2px(channels));
    lcd->box.size = mm2px(Vec(8.197, 8.197));
    lcd->module = module;
    addChild(lcd);

    // ... expander indicators
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_vco::XLL_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_vco::XRR_LIGHT));
}

void sn_vcoWidget::appendContextMenu(Menu *menu) {
    sn_vco *module = getModule<sn_vco>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-vco settings"));

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

Model *model_sn_vco = createModel<sn_vco, sn_vcoWidget>("sn-vco");
