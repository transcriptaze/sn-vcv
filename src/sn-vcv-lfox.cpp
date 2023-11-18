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
    configSwitch(M_PARAM, 1.f, 5.f, 1.f, "m", {"1", "2", "3", "4", "5"});

    getParamQuantity(M_PARAM)->randomizeEnabled = false;

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

    json_object_set_new(root, "k-rate", json_integer(krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));

    return root;
}

void sn_vcv_lfox::dataFromJson(json_t *root) {
    json_t *krate = json_object_get(root, "k-rate");
    json_t *aux_mode = json_object_get(root, "aux-mode");

    if (krate) {
        int v = json_integer_value(krate);

        if (v >= 0 && v < 4) {
            this->krate = json_integer_value(krate);
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
    // unsigned int krate = KRATE[this->krate];

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

    // // ... audio
    // if (msg != NULL) {
    //     channels = msg->channels;

    //     for (int ch = 0; ch < channels; ch++) {
    //         LFO[ch].phase = msg->LFO[ch].phase;
    //         LFO[ch].out.sum = msg->LFO[ch].out;
    //     }

    //     aux.phase = msg->AUX.phase;
    //     aux.out.sum = msg->AUX.out;

    //     debug = msg->debug;
    // } else {
    //     for (int ch = 0; ch < channels; ch++) {
    //         LFO[ch].phase = 0.0f;
    //         LFO[ch].out.sum = 0.0f;
    //     }

    //     aux.phase = 0.0f;
    //     aux.out.sum = 0.0f;
    // }

    // if (debug) {
    //     INFO("snyth-lfo-x  k-rate:%d", krate);
    // }

    // // ... get params and recompute transform matrix
    // bool recalculate = false;

    // update++;

    // if (update >= krate) {
    //     settings(debug);
    //     recompute(debug);
    //     recalculate = true;
    //     update = 0;
    // }

    // // ... LFO
    // float m = parameters.m;
    // float A = parameters.A;
    // float φ = matrix.φ;

    // float pʼ = matrix.pʼ;
    // float qʼ = matrix.qʼ;
    // float rʼ = matrix.rʼ;
    // float sʼ = matrix.sʼ;
    // float tʼ = matrix.tʼ;
    // float uʼ = matrix.uʼ;

    // if ((outputs[LFO_OUTPUT].isConnected() || outputs[SUM_OUTPUT].isConnected()) && recalculate) {
    //     for (int ch = 0; ch < channels; ch++) {
    //         float α = 2.0f * M_PI * LFO[ch].phase;

    //         float 𝜓 = 0.0f;
    //         float αʼ = m * (α + 𝜓) - φ;

    //         float x = std::cos(αʼ);
    //         float y = std::sin(αʼ);
    //         float xʼ = pʼ * x - qʼ * y + rʼ;
    //         float yʼ = sʼ * x + tʼ * y + uʼ;

    //         float r = std::hypot(xʼ, yʼ);
    //         float sn = r > 0.0f ? yʼ / r : 0.0f;

    //         LFO[ch].out.osc = sn;
    //         LFO[ch].out.sum += A * sn;
    //     }
    // }

    // if (outputs[LFO_OUTPUT].isConnected() || outputs[SUM_OUTPUT].isConnected()) {
    //     for (int ch = 0; ch < channels; ch++) {
    //         outputs[LFO_OUTPUT].setVoltage(5.f * LFO[ch].out.osc, ch);
    //         outputs[SUM_OUTPUT].setVoltage(5.f * LFO[ch].out.sum, ch);
    //     }

    //     outputs[LFO_OUTPUT].setChannels(channels);
    //     outputs[SUM_OUTPUT].setChannels(channels);
    // }

    // // ... AUX
    // if (outputs[AUX_OUTPUT].isConnected() || expanded) {
    //     float α = 2.0f * M_PI * aux.phase;

    //     float 𝜓 = 0.0f;
    //     float αʼ = m * (α + 𝜓) - φ;

    //     float x = std::cos(αʼ);
    //     float y = std::sin(αʼ);
    //     float xʼ = pʼ * x - qʼ * y + rʼ;
    //     float yʼ = sʼ * x + tʼ * y + uʼ;

    //     float r = std::hypot(xʼ, yʼ);
    //     float sn = r > 0.0f ? yʼ / r : 0.0f;

    //     aux.out.osc = sn;
    //     aux.out.sum += A * sn;
    // }

    // if (outputs[AUX_OUTPUT].isConnected()) {
    //     switch (aux.mode) {
    //     case POLY:
    //         outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.osc, 0);
    //         outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.sum, 1);
    //         outputs[AUX_OUTPUT].setChannels(2);
    //         break;

    //     case SUM:
    //         outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.sum);
    //         outputs[AUX_OUTPUT].setChannels(1);
    //         break;

    //     default:
    //         outputs[AUX_OUTPUT].setVoltage(5.f * aux.out.osc);
    //         outputs[AUX_OUTPUT].setChannels(1);
    //     }
    // }

    // ... update expanders
    {
        sn_lfo_message *msg = expanders.left.producer();

        if (msg != NULL) {
            msg->linked = msgR != NULL && msgR->linked;
            msg->channels = channels;

            for (int ch = 0; ch < channels; ch++) {
                msg->LFO[ch].phase = LFO[ch].phase;
                msg->LFO[ch].out = LFO[ch].out.sum;
            }

            msg->AUX.phase = aux.phase;
            msg->AUX.out = aux.out.sum;

            expanders.left.flip();
        }
    }

    {
        sn_lfo_message *msg = expanders.right.producer();

        if (msg != NULL) {
            msg->linked = msgL != NULL && msgL->linked;
            msg->channels = channels;

            for (int ch = 0; ch < channels; ch++) {
                msg->LFO[ch].phase = LFO[ch].phase;
                msg->LFO[ch].out = LFO[ch].out.sum;
            }

            msg->AUX.phase = aux.phase;
            msg->AUX.out = aux.out.sum;

            expanders.right.flip();
        }
    }
}

void sn_vcv_lfox::settings(bool debug) {
    // // ... param values
    // float e = params[ECCENTRICITY_PARAM].getValue();
    // float s = params[SENSITIVITY_PARAM].getValue();
    // float θ = params[ROTATION_PARAM].getValue();
    // float A = params[AMPLITUDE_PARAM].getValue();
    // float δx = params[DX_PARAM].getValue();
    // float δy = params[DY_PARAM].getValue();
    // float Φ = params[PHI_PARAM].getValue();
    // float m = params[M_PARAM].getValue();

    // // ... rescale, sanitize and package
    // parameters.ε = std::tanh(s * e);
    // parameters.θ = clamp(θ, -89.95f, +89.95f) * M_PI / 180.0f;
    // parameters.A = A;
    // parameters.δx = δx;
    // parameters.δy = δy;
    // parameters.Φ = Φ;
    // parameters.m = m;

    // if (debug) {
    //     INFO("snyth-lfo  A:%.3f", A);
    // }
}

// ... recompute transform matrix
void sn_vcv_lfox::recompute(bool debug) {
    // float ε = parameters.ε;
    // float θ = parameters.θ;
    // float A = parameters.A;
    // float δx = parameters.δx;
    // float δy = parameters.δy;
    // float Φ = parameters.Φ;

    // float εʼ = std::sqrt(1.0f - ε * ε);
    // float a = (ε < 0.0f) ? εʼ : 1.0f;
    // float b = (ε > 0.0f) ? εʼ : 1.0f;

    // float cosθ = std::cos(θ);
    // float sinθ = std::sin(θ);

    // float u = std::atan(-b * std::tan(θ) / a);
    // float v = std::atan((b / std::tan(θ)) * a);
    // float tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    // float ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    // float δxʼ = tx * δx;
    // float δyʼ = ty * δy;

    // matrix.pʼ = A * a * cosθ;
    // matrix.qʼ = A * b * sinθ;
    // matrix.rʼ = A * δxʼ;
    // matrix.sʼ = A * a * sinθ;
    // matrix.tʼ = A * b * cosθ;
    // matrix.uʼ = A * δyʼ;
    // matrix.φ = phi(a, b, θ, Φ);
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
                                             &module->krate));

    menu->addChild(createIndexPtrSubmenuItem("aux-mode",
                                             AUX_MODES,
                                             &module->aux.mode));
}

Model *modelSn_vcv_lfox = createModel<sn_vcv_lfox, sn_vcv_lfoxWidget>("sn-vcv-lfox");
