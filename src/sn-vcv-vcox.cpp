#include "sn-vcv-vcox.hpp"

const int sn_vcv_vcox::CHANNELS = 1;

sn_vcv_vcox::sn_vcv_vcox() {
    // ... params
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    configParam(ECCENTRICITY_PARAM, -1.0f, +1.0f, 0.0f, "ε");
    configParam(SENSITIVITY_PARAM, 0.0f, +5.0f, 2.5f, "s");
    configParam(ROTATION_PARAM, -90.0f, +90.0f, 0.0f, "θ");
    configParam(AMPLITUDE_PARAM, 0.0f, +1.0f, 1.0f, "a");
    configParam(DX_PARAM, -1.0f, +1.0f, 0.0f, "δx");
    configParam(DY_PARAM, -1.0f, +1.0f, 0.0f, "δy");
    configSwitch(M_PARAM, 1.f, 5.f, 2.f, "m", {"1", "2", "3", "4", "5"});
    configParam(ATT_PARAM, 0.0f, +1.0f, 1.0f, "ɡ");

    getParamQuantity(PLACEHOLDER_PARAM)->randomizeEnabled = false;
    getParamQuantity(M_PARAM)->randomizeEnabled = false;
    getParamQuantity(ATT_PARAM)->randomizeEnabled = false;

    // ... inputs
    configInput(ECCENTRICITY_INPUT, "±5V ε");
    configInput(SENSITIVITY_INPUT, "±5V s");
    configInput(ROTATION_INPUT, "±5V Φ");
    configInput(AMPLITUDE_INPUT, "±5V a");
    configInput(DX_INPUT, "±5V δx");
    configInput(DY_INPUT, "±5V δy");

    // ... outputs
    configOutput(VCO_OUTPUT, "VCO");
    configOutput(VCO_SUM_OUTPUT, "VCO-Σ");
    configOutput(AUX_OUTPUT, "AUX");

    // ... expanders
    getLeftExpander().producerMessage = &expanders.left.messages[0];
    getLeftExpander().consumerMessage = &expanders.left.messages[1];

    getRightExpander().producerMessage = &expanders.right.messages[0];
    getRightExpander().consumerMessage = &expanders.right.messages[1];
}

bool sn_vcv_vcox::isLinkedLeft() {
    return expanders.left.linked;
}

bool sn_vcv_vcox::isLinkedRight() {
    return expanders.right.linked;
}

json_t *sn_vcv_vcox::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));
    json_object_set_new(root, "aux-gain", json_integer(aux.gain));

    return root;
}

void sn_vcv_vcox::dataFromJson(json_t *root) {
    json_t *krate = json_object_get(root, "k-rate");
    json_t *aux_mode = json_object_get(root, "aux-mode");
    json_t *gain = json_object_get(root, "aux-gain");

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

void sn_vcv_vcox::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    expanders.linkedLeft = left && left->model == modelSn_vcv_vco;
    expanders.linkedRight = right && right->model == modelSn_vcv_vco;

    if (left && left->model == modelSn_vcv_vcox) {
        expanders.left.module = left;
    } else {
        expanders.left.module = NULL;
    }

    if (right && right->model == modelSn_vcv_vcox) {
        expanders.right.module = right;
    } else {
        expanders.right.module = NULL;
    }
}

void sn_vcv_vcox::process(const ProcessArgs &args) {
    int channels = CHANNELS;
    bool expanded = expanders.left.module != NULL || expanders.right.module != NULL;

    // ... expanders
    sn_vco_message *msg = NULL;
    sn_vco_message *msgL = NULL;
    sn_vco_message *msgR = NULL;

    if (expanders.linkedLeft || (expanders.left.module != NULL)) {
        msgL = (sn_vco_message *)getLeftExpander().consumerMessage;
    }

    if (expanders.linkedRight || (expanders.right.module != NULL)) {
        msgR = (sn_vco_message *)getRightExpander().consumerMessage;
    }

    if (expanders.linkedLeft) {
        msg = msgL;
    } else if (expanders.left.module && msgL && msgL->linked) {
        msg = msgL;
    } else if (expanders.linkedRight) {
        msg = msgR;
    } else if (expanders.right.module && msgR && msgR->linked) {
        msg = msgR;
    }

    bool xll = false;
    bool xlr = false;
    bool xrl = false;
    bool xrr = false;

    if (expanders.linkedLeft) {
        xll = true;
    } else if (expanders.left.module && msgL && msgL->linked) {
        xll = true;
    } else if (expanders.linkedRight) {
        xrr = true;
    } else if (expanders.right.module && msgR && msgR->linked) {
        xrr = true;
    }

    if (xll && expanders.right.module) {
        xrl = true;
    } else if (xrr && expanders.left.module) {
        xlr = true;
    }

    expanders.left.linked = xll;
    expanders.right.linked = xrr;

    lights[XLL_LIGHT].setBrightnessSmooth(xll ? 1.f : 0.f, args.sampleTime);
    lights[XLR_LIGHT].setBrightnessSmooth(xlr ? 1.f : 0.f, args.sampleTime);
    lights[XRL_LIGHT].setBrightnessSmooth(xrl ? 1.f : 0.f, args.sampleTime);
    lights[XRR_LIGHT].setBrightnessSmooth(xrr ? 1.f : 0.f, args.sampleTime);

    // ... get params and recompute transform matrix
    update.count--;

    if (update.count <= 0) {
        recompute();
        update.count = KRATE[update.krate];
    }

    // ... generate
    if (msg) {
        channels = msg->channels;

        for (int ch = 0; ch < channels; ch++) {
            vco[ch].phase = msg->vco[ch].phase;
            vco[ch].velocity = msg->vco[ch].velocity;
            vco[ch].out.sum = msg->vco[ch].out;
        }

        aux.phase = msg->aux.phase;
        aux.out.sum = msg->aux.out;
    } else {
        for (int ch = 0; ch < 16; ch++) {
            vco[ch].phase = 0.0f;
            vco[ch].velocity = 0.f;
        }

        aux.phase = 0.0f;
        aux.out.sum = 0.0f;
    }

    processVCO(args, channels, expanded);
    processAUX(args, expanded);

    // ... update expanders
    {
        sn_vco_message *msg;

        if ((msg = expanders.left.producer()) != NULL) {
            bool linked = msgR != NULL && msgR->linked;
            msg->set(linked, channels, vco, aux);
            expanders.left.flip();
        }

        if ((msg = expanders.right.producer()) != NULL) {
            bool linked = msgL != NULL && msgL->linked;
            msg->set(linked, channels, vco, aux);
            expanders.right.flip();
        }
    }
}

void sn_vcv_vcox::processVCO(const ProcessArgs &args, int channels, bool expanded) {
    bool connected = outputs[VCO_OUTPUT].isConnected() | outputs[VCO_SUM_OUTPUT].isConnected();
    float gain = params[ATT_PARAM].getValue();

    if (connected || expanded) {
        for (int ch = 0; ch < channels; ch++) {
            float α = vco[ch].phase * 2.0f * M_PI;
            float υ = sn.υ(α);

            vco[ch].out.vco = υ;
            vco[ch].out.sum += sn.A * υ;
        }
    }

    if (outputs[VCO_OUTPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[VCO_OUTPUT].setVoltage(5.f * vco[ch].velocity * vco[ch].out.vco, ch);
        }

        outputs[VCO_OUTPUT].setChannels(channels);
    }

    if (outputs[VCO_SUM_OUTPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[VCO_SUM_OUTPUT].setVoltage(5.f * vco[ch].velocity * gain * vco[ch].out.sum, ch);
        }

        outputs[VCO_SUM_OUTPUT].setChannels(channels);
    }
}

void sn_vcv_vcox::processAUX(const ProcessArgs &args, bool expanded) {
    if (outputs[AUX_OUTPUT].isConnected() || expanded) {
        float α = aux.phase * 2.0f * M_PI;
        float υ = sn.υ(α);

        aux.out.osc = sn.A * υ;
        aux.out.sum += sn.A * υ;
    } else {
        aux.out.osc = 0.0f;
        aux.out.sum = 0.0f;
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

void sn_vcv_vcox::recompute() {
    // ... param values
    float e = params[ECCENTRICITY_PARAM].getValue();
    float s = params[SENSITIVITY_PARAM].getValue();
    float θ = params[ROTATION_PARAM].getValue();
    float A = params[AMPLITUDE_PARAM].getValue();
    float δx = params[DX_PARAM].getValue();
    float δy = params[DY_PARAM].getValue();
    float m = params[M_PARAM].getValue();

    // ... add inputs
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

    // ... recalculate SN transform
    sn.ε = std::tanh(s * e);
    sn.θ = θ * M_PI / 180.0f;
    sn.A = clamp(A, 0.0f, +1.0f);
    sn.δx = clamp(δx, -1.0f, +1.0f);
    sn.δy = clamp(δy, -1.0f, +1.0f);
    sn.m = m;

    sn.recompute();
}

sn_vcv_vcoxWidget::sn_vcv_vcoxWidget(sn_vcv_vcox *module) {
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
    Vec param_att(right, top + 0.5 * dh);

    Vec aux(right, top + 4 * dh);
    Vec vco(right, top + 6 * dh);
    Vec vcox(right, top + 7 * dh);

    Vec xll(2.54, 11.43 + 2.54);
    Vec xrr(43.18, 11.43 + 2.54);

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-vcv-vcox.svg"),
                         asset::plugin(pluginInstance, "res/sn-vcv-vcox-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... parameters
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_e), module, sn_vcv_vcox::ECCENTRICITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_s), module, sn_vcv_vcox::SENSITIVITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_θ), module, sn_vcv_vcox::ROTATION_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_A), module, sn_vcv_vcox::AMPLITUDE_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δx), module, sn_vcv_vcox::DX_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δy), module, sn_vcv_vcox::DY_INPUT));

    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_e), module, sn_vcv_vcox::ECCENTRICITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_s), module, sn_vcv_vcox::SENSITIVITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_θ), module, sn_vcv_vcox::ROTATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_A), module, sn_vcv_vcox::AMPLITUDE_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δx), module, sn_vcv_vcox::DX_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δy), module, sn_vcv_vcox::DY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_m), module, sn_vcv_vcox::M_PARAM));

    // ... ATT
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_att), module, sn_vcv_vcox::ATT_PARAM));

    // ... aux, VCO and VCO-Σ outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(aux), module, sn_vcv_vcox::AUX_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(vco), module, sn_vcv_vcox::VCO_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(vcox), module, sn_vcv_vcox::VCO_SUM_OUTPUT));

    // ... expander indicators
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xll), module, sn_vcv_vcox::XLL_LIGHT));
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_vcv_vcox::XLR_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_vcv_vcox::XRL_LIGHT));
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xrr), module, sn_vcv_vcox::XRR_LIGHT));
}

void sn_vcv_vcoxWidget::appendContextMenu(Menu *menu) {
    sn_vcv_vcox *module = getModule<sn_vcv_vcox>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-vcox settings"));

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

Model *modelSn_vcv_vcox = createModel<sn_vcv_vcox, sn_vcv_vcoxWidget>("sn-vcv-vcox");