#include "sn-vcv-lfox.hpp"

const int sn_vcv_lfox::CHANNELS = 1;

sn_vcv_lfox::sn_vcv_lfox() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    configParam(ECCENTRICITY_PARAM, -1.0f, +1.0f, 0.0f, "ε");
    configParam(SENSITIVITY_PARAM, 0.0f, +5.0f, 2.5f, "s");
    configParam(ROTATION_PARAM, -90.0f, +90.0f, 0.0f, "θ");
    configParam(AMPLITUDE_PARAM, 0.0f, +1.0f, 1.0f, "a");
    configParam(DX_PARAM, -1.0f, +1.0f, 0.0f, "δx");
    configParam(DY_PARAM, -1.0f, +1.0f, 0.0f, "δy");
    configParam(PHI_PARAM, -90.0f, +90.0f, 0.0f, "Φ");
    configSwitch(M_PARAM, 1.f, 5.f, 2.f, "m", {"1", "2", "3", "4", "5"});
    configParam(ATT_PARAM, 0.0f, +1.0f, 1.0f, "ɡ");

    getParamQuantity(M_PARAM)->randomizeEnabled = false;
    getParamQuantity(ATT_PARAM)->randomizeEnabled = false;

    configOutput(LFO_OUTPUT, "LFO");
    configOutput(SUM_OUTPUT, "Summed");
    configOutput(AUX_OUTPUT, "AUX");

    getLeftExpander().producerMessage = &expanders.left.messages[0];
    getLeftExpander().consumerMessage = &expanders.left.messages[1];

    getRightExpander().producerMessage = &expanders.right.messages[0];
    getRightExpander().consumerMessage = &expanders.right.messages[1];
}

json_t *sn_vcv_lfox::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));
    json_object_set_new(root, "aux-gain", json_integer(aux.gain));

    return root;
}

void sn_vcv_lfox::dataFromJson(json_t *root) {
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

void sn_vcv_lfox::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    expanders.linkLeft = left && left->model == modelSn_vcv_lfo;
    expanders.linkRight = right && right->model == modelSn_vcv_lfo;

    if (left && left->model == modelSn_vcv_lfox) {
        expanders.left.module = left;
    } else {
        expanders.left.module = NULL;
    }

    if (right && right->model == modelSn_vcv_lfox) {
        expanders.right.module = right;
    } else {
        expanders.right.module = NULL;
    }
}

void sn_vcv_lfox::process(const ProcessArgs &args) {
    int channels = CHANNELS;

    // ... expanders
    bool expanded = expanders.left.module != NULL || expanders.right.module != NULL;
    sn_lfo_message *msg = NULL;
    sn_lfo_message *msgL = NULL;
    sn_lfo_message *msgR = NULL;

    if (expanders.linkLeft || expanders.left.module != NULL) {
        msgL = (sn_lfo_message *)getLeftExpander().consumerMessage;
    }

    if (expanders.linkRight || expanders.right.module != NULL) {
        msgR = (sn_lfo_message *)getRightExpander().consumerMessage;
    }

    if (expanders.linkLeft) {
        msg = msgL;
    } else if (expanders.left.module && msgL && msgL->linked) {
        msg = msgL;
    } else if (expanders.linkRight) {
        msg = msgR;
    } else if (expanders.right.module && msgR && msgR->linked) {
        msg = msgR;
    }

    // ... expander indicators
    bool xll = false;
    bool xlr = false;
    bool xrl = false;
    bool xrr = false;

    if (expanders.linkLeft) {
        xll = true;
    } else if (expanders.left.module && msgL && msgL->linked) {
        xll = true;
    } else if (expanders.linkRight) {
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
    bool recalculate = false;

    update.count--;

    if (update.count <= 0) {
        recompute();
        recalculate = true;
        update.count = KRATE[update.krate];
    }

    // ... generate
    if (msg != NULL) {
        channels = msg->channels;

        for (int ch = 0; ch < channels; ch++) {
            lfo[ch].phase = msg->lfo[ch].phase;
            lfo[ch].out.sum = msg->lfo[ch].out;
        }

        aux.phase = msg->aux.phase;
        aux.out.sum = msg->aux.out;
    } else {
        for (int ch = 0; ch < channels; ch++) {
            lfo[ch].phase = 0.0f;
            lfo[ch].out.sum = 0.0f;
        }

        aux.phase = 0.0f;
        aux.out.sum = 0.0f;
    }

    processLFO(args, channels, expanded, recalculate);
    processAUX(args, expanded);

    // ... update expanders
    {
        sn_lfo_message *msg;

        if ((msg = expanders.left.producer()) != NULL) {
            bool linked = msgR != NULL && msgR->linked;
            msg->set(linked, channels, lfo, aux);
            expanders.left.flip();
        }

        if ((msg = expanders.right.producer()) != NULL) {
            bool linked = msgL != NULL && msgL->linked;
            msg->set(linked, channels, lfo, aux);
            expanders.right.flip();
        }
    }
}

void sn_vcv_lfox::recompute() {
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

void sn_vcv_lfox::processLFO(const ProcessArgs &args, int channels, bool expanded, bool recalculate) {
    float gain = params[ATT_PARAM].getValue();

    if ((outputs[LFO_OUTPUT].isConnected() || outputs[SUM_OUTPUT].isConnected() || expanded) && recalculate) {
        for (int ch = 0; ch < channels; ch++) {
            float α = 2.0f * M_PI * lfo[ch].phase;
            float υ = sn.υ(α);

            lfo[ch].out.lfo = υ;
        }
    }

    for (int ch = 0; ch < channels; ch++) {
        lfo[ch].out.sum += sn.A * lfo[ch].out.lfo;
    }

    if (outputs[LFO_OUTPUT].isConnected() || outputs[SUM_OUTPUT].isConnected()) {
        for (int ch = 0; ch < channels; ch++) {
            outputs[LFO_OUTPUT].setVoltage(5.f * lfo[ch].out.lfo, ch);
            outputs[SUM_OUTPUT].setVoltage(5.f * gain * lfo[ch].out.sum, ch);
        }

        outputs[LFO_OUTPUT].setChannels(channels);
        outputs[SUM_OUTPUT].setChannels(channels);
    }
}

void sn_vcv_lfox::processAUX(const ProcessArgs &args, bool expanded) {
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

bool sn_vcv_lfox::isLinkedLeft() {
    return expanders.left.linked;
}

bool sn_vcv_lfox::isLinkedRight() {
    return expanders.right.linked;
}

sn_vcv_lfoxWidget::sn_vcv_lfoxWidget(sn_vcv_lfox *module) {
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
    Vec param_att(right, top + 0.5 * dh);

    Vec aux(right, top + 4 * dh);
    Vec lfo(right, top + 6 * dh);
    Vec sum(right, top + 7 * dh);

    Vec xll(2.54, 13.97);
    Vec xrr(33.02, 13.97);

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-vcv-lfox.svg"),
                         asset::plugin(pluginInstance, "res/sn-vcv-lfox-dark.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... parameters
    addParam(createParamCentered<RoundBlackKnob>(mm2px(e), module, sn_vcv_lfox::ECCENTRICITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(s), module, sn_vcv_lfox::SENSITIVITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(θ), module, sn_vcv_lfox::ROTATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(A), module, sn_vcv_lfox::AMPLITUDE_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(δx), module, sn_vcv_lfox::DX_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(δy), module, sn_vcv_lfox::DY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(𝜓), module, sn_vcv_lfox::PHI_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(m), module, sn_vcv_lfox::M_PARAM));

    // ... ATT
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_att), module, sn_vcv_lfox::ATT_PARAM));

    // ... outputs
    addOutput(createOutputCentered<PJ301MPort>(mm2px(lfo), module, sn_vcv_lfox::LFO_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(sum), module, sn_vcv_lfox::SUM_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(aux), module, sn_vcv_lfox::AUX_OUTPUT));

    // ... expander indicators
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xll), module, sn_vcv_lfox::XLL_LIGHT));
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_vcv_lfox::XLR_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_vcv_lfox::XRL_LIGHT));
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xrr), module, sn_vcv_lfox::XRR_LIGHT));
}

void sn_vcv_lfoxWidget::appendContextMenu(Menu *menu) {
    sn_vcv_lfox *module = getModule<sn_vcv_lfox>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-lfo-x settings"));

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

Model *modelSn_vcv_lfox = createModel<sn_vcv_lfox, sn_vcv_lfoxWidget>("sn-vcv-lfox");
