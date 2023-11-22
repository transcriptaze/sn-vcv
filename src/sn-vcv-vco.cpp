#include "sn-vcv-vco.hpp"
#include "sn-vcv-vcox.hpp"

const int sn_vcv_vco::CHANNELS = 1;
const float sn_vcv_vco::VELOCITY = 1.0f;

sn_vcv_vco::sn_vcv_vco() {
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

    configInput(ECCENTRICITY_INPUT, "±5V ε");
    configInput(SENSITIVITY_INPUT, "0-10V s");
    configInput(ROTATION_INPUT, "±5V Φ");
    configInput(AMPLITUDE_INPUT, "0-10V a");
    configInput(DX_INPUT, "±5V δx");
    configInput(DY_INPUT, "±5V δy");

    configInput(PITCH_INPUT, "1V/octave pitch");
    configInput(VELOCITY_INPUT, "0-10V velocity");

    configOutput(VCO_OUTPUT, "VCO");
    configOutput(AUX_OUTPUT, "AUX");
    configOutput(AUX_TRIGGER, "Trigger");

    // ... reset trigger
    trigger.reset();
}

json_t *sn_vcv_vco::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));
    json_object_set_new(root, "aux-gain", json_integer(aux.gain));

    return root;
}

void sn_vcv_vco::dataFromJson(json_t *root) {
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

void sn_vcv_vco::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    if (left && left->model == modelSn_vcv_vcox) {
        expanders.left = left;
    } else {
        expanders.left = NULL;
    }

    if (right && right->model == modelSn_vcv_vcox) {
        expanders.right = right;
    } else {
        expanders.right = NULL;
    }
}

void sn_vcv_vco::process(const ProcessArgs &args) {
    int channels = this->channels();

    // ... expanders
    bool expanded = expanders.left != NULL || expanders.right != NULL;
    bool xll = false;
    bool xrr = expanders.right;

    if (expanders.left) {
        sn_vcv_vcox *x = (sn_vcv_vcox *)expanders.left;
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
    if (expanders.left) {
        sn_vco_message *msg = (sn_vco_message *)expanders.left->getRightExpander().producerMessage;

        if (msg != NULL) {
            msg->linked = true;
            msg->channels = channels;

            for (int ch = 0; ch < 16; ch++) {
                msg->vco[ch].phase = VCO[ch].phase;
                msg->vco[ch].velocity = VCO[ch].velocity;
                msg->vco[ch].out = VCO[ch].out;
            }

            msg->aux.phase = aux.phase;
            msg->aux.out = aux.out.sum;

            expanders.left->getRightExpander().requestMessageFlip();
        }
    }

    if (expanders.right) {
        sn_vco_message *msg = (sn_vco_message *)expanders.right->getLeftExpander().producerMessage;

        if (msg != NULL) {
            msg->linked = true;
            msg->channels = channels;

            for (int ch = 0; ch < 16; ch++) {
                msg->vco[ch].phase = VCO[ch].phase;
                msg->vco[ch].velocity = VCO[ch].velocity;
                msg->vco[ch].out = VCO[ch].out;
            }

            msg->aux.phase = aux.phase;
            msg->aux.out = aux.out.sum;

            expanders.right->getLeftExpander().requestMessageFlip();
        }
    }
}

void sn_vcv_vco::processVCO(const ProcessArgs &args, int channels, bool expanded) {
    bool vco = outputs[VCO_OUTPUT].isConnected();

    // ... convert pitch CV to instantaneous frequency
    for (int ch = 0; ch < channels; ch++) {
        float pitch = inputs[PITCH_INPUT].getPolyVoltage(ch);
        float f = dsp::FREQ_C4 * std::pow(2.f, pitch);

        VCO[ch].phase += f * args.sampleTime;
        if (VCO[ch].phase >= 1.f) {
            VCO[ch].phase -= 1.f;
        }
    }

    // ... generate
    if (vco || expanded) {
        for (int ch = 0; ch < channels; ch++) {
            float α = VCO[ch].phase * 2.0f * M_PI;
            float υ = sn.υ(α);

            VCO[ch].out = sn.A * υ;
            VCO[ch].velocity = velocity(ch);
        }
    }

    if (vco) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[VCO_OUTPUT].setVoltage(5.f * VCO[ch].velocity * VCO[ch].out, ch);
        }

        outputs[VCO_OUTPUT].setChannels(channels);
    }
}

void sn_vcv_vco::processAUX(const ProcessArgs &args, bool expanded) {
    aux.phase += AUX_FREQUENCY * args.sampleTime;
    if (aux.phase >= 1.f) {
        aux.phase -= 1.f;

        if (outputs[AUX_TRIGGER].isConnected()) {
            trigger.trigger(0.001f);
        }
    }

    bool triggered = trigger.process(args.sampleTime);

    if (outputs[AUX_OUTPUT].isConnected() || expanded) {
        float α = aux.phase * 2.0f * M_PI;
        float υ = sn.υ(α);

        aux.out.osc = υ;
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

void sn_vcv_vco::recompute() {
    // ... param values
    float e = params[ECCENTRICITY_PARAM].getValue();
    float s = params[SENSITIVITY_PARAM].getValue();
    float θ = params[ROTATION_PARAM].getValue();
    float A = params[AMPLITUDE_PARAM].getValue();
    float δx = params[DX_PARAM].getValue();
    float δy = params[DY_PARAM].getValue();
    float m = params[M_PARAM].getValue();

    // ... override with inputs
    if (inputs[ECCENTRICITY_INPUT].isConnected()) {
        e = clamp(inputs[ECCENTRICITY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    }

    if (inputs[SENSITIVITY_INPUT].isConnected()) {
        s = clamp(inputs[SENSITIVITY_INPUT].getVoltage() / 2.0f, 0.0f, +5.0f);
    }

    if (inputs[ROTATION_INPUT].isConnected()) {
        θ = clamp(90.0f * inputs[ROTATION_INPUT].getVoltage() / 5.0f, -90.0f, +90.0f);
    }

    if (inputs[AMPLITUDE_INPUT].isConnected()) {
        A = clamp(inputs[AMPLITUDE_INPUT].getVoltage() / 10.0f, 0.0f, 1.0f);
    }

    if (inputs[DX_INPUT].isConnected()) {
        δx = clamp(inputs[DX_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    }

    if (inputs[DY_INPUT].isConnected()) {
        δy = clamp(inputs[DY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    }

    // ... set internal SN parameters
    sn.ε = std::tanh(s * e);
    sn.θ = clamp(θ, -89.95f, +89.95f) * M_PI / 180.0f;
    sn.A = A;
    sn.δx = δx;
    sn.δy = δy;
    sn.m = m;

    sn.recompute();
}

int sn_vcv_vco::channels() {
    return inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;
}

float sn_vcv_vco::velocity(int channel) {
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

sn_vcv_vcoWidget::sn_vcv_vcoWidget(sn_vcv_vco *module) {
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
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-vcv-vco.svg"),
                         asset::plugin(pluginInstance, "res/sn-vcv-vco-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... e
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_e), module, sn_vcv_vco::ECCENTRICITY_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_e), module, sn_vcv_vco::ECCENTRICITY_PARAM));

    // ... s
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_s), module, sn_vcv_vco::SENSITIVITY_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_s), module, sn_vcv_vco::SENSITIVITY_PARAM));

    // ... θ
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_θ), module, sn_vcv_vco::ROTATION_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_θ), module, sn_vcv_vco::ROTATION_PARAM));

    // ... A
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_A), module, sn_vcv_vco::AMPLITUDE_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_A), module, sn_vcv_vco::AMPLITUDE_PARAM));

    // ... δx
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δx), module, sn_vcv_vco::DX_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δx), module, sn_vcv_vco::DX_PARAM));

    // ... δy
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δy), module, sn_vcv_vco::DY_INPUT));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δy), module, sn_vcv_vco::DY_PARAM));

    // ... m,
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_m), module, sn_vcv_vco::M_PARAM));

    // ... pitch and velocity inputs
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(pitch), module, sn_vcv_vco::PITCH_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(velocity), module, sn_vcv_vco::VELOCITY_INPUT));

    // ... aux, trigger and VCO outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(aux), module, sn_vcv_vco::AUX_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(trigger), module, sn_vcv_vco::AUX_TRIGGER));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(vco), module, sn_vcv_vco::VCO_OUTPUT));

    // ... channels
    sn_vcv_vco_channels *lcd = createWidget<sn_vcv_vco_channels>(mm2px(channels));
    lcd->box.size = mm2px(Vec(8.197, 8.197));
    lcd->module = module;
    addChild(lcd);

    // ... expander indicators
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_vcv_vco::XLL_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_vcv_vco::XRR_LIGHT));
}

void sn_vcv_vcoWidget::appendContextMenu(Menu *menu) {
    sn_vcv_vco *module = getModule<sn_vcv_vco>();

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

Model *modelSn_vcv_vco = createModel<sn_vcv_vco, sn_vcv_vcoWidget>("sn-vcv-vco");