#include "sn-vcox.hpp"

const int sn_vcox::CHANNELS = 1;

sn_vcox::sn_vcox() {
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

bool sn_vcox::isLinkedLeft() {
    return expanders.left.linked;
}

bool sn_vcox::isLinkedRight() {
    return expanders.right.linked;
}

json_t *sn_vcox::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "k-rate", json_integer(update.krate));
    json_object_set_new(root, "aux-mode", json_integer(aux.mode));
    json_object_set_new(root, "aux-gain", json_integer(aux.gain));

    return root;
}

void sn_vcox::dataFromJson(json_t *root) {
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

void sn_vcox::onExpanderChange(const ExpanderChangeEvent &e) {
    Module *left = getLeftExpander().module;
    Module *right = getRightExpander().module;

    expanders.linkedLeft = left && left->model == model_sn_vco;
    expanders.linkedRight = right && right->model == model_sn_vco;

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

void sn_vcox::process(const ProcessArgs &args) {
    int channels = CHANNELS;
    ANTIALIAS antialias = NONE;
    DCBLOCK dcblocking = DCBLOCK_NONE;
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

    // ... generate
    if (msg) {
        channels = msg->channels;
        antialias = msg->antialias;
        dcblocking = msg->dcblocking;
    }

    recompute(args, dcblocking);

    if (msg) {
        for (int ch = 0; ch < channels; ch++) {
            vco[ch].velocity = msg->vco[ch].velocity;

            for (int i = 0; i < 4; i++) {
                vco[ch].phase[i] = msg->vco[ch].phase[i];
                vco[ch].out.sum[i] = msg->vco[ch].out[i];
            }
        }

        aux.phase = msg->aux.phase;
        aux.out.sum = msg->aux.out;

        fft.fftx.rate = msg->fft.rate;
        for (int ch = 0; ch < channels; ch++) {
            fft.fftx.frequency[ch] = msg->fft.frequency[ch];
            fft.fftx.velocity[ch] = msg->fft.velocity[ch];
        }

    } else {
        for (int ch = 0; ch < 16; ch++) {
            vco[ch].velocity = 0.f;
            for (int i = 0; i < 4; i++) {
                vco[ch].phase[i] = 0.f;
            }
        }

        aux.phase = 0.0f;
        aux.out.sum = 0.0f;
        fft.fftx.rate = FFT_RATE::OFF;

        for (int ch = 0; ch < 16; ch++) {
            fft.fftx.frequency[ch] = dsp::FREQ_C4;
            fft.fftx.velocity[ch] = 0.f;
        }
    }

    processVCO(args, channels, antialias, dcblocking, expanded);
    processAUX(args, expanded);

    // ... update expanders
    {
        sn_vco_message *msg;

        if ((msg = expanders.left.producer()) != NULL) {
            bool linked = msgR != NULL && msgR->linked;
            msg->set(linked, channels, antialias, dcblocking, vco, aux, fft.fftx);
            expanders.left.flip();
        }

        if ((msg = expanders.right.producer()) != NULL) {
            bool linked = msgL != NULL && msgL->linked;
            msg->set(linked, channels, antialias, dcblocking, vco, aux, fft.fftx);
            expanders.right.flip();
        }
    }
}

void sn_vcox::processVCO(const ProcessArgs &args, size_t channels, ANTIALIAS antialias, DCBLOCK dcblocking, bool expanded) {
    bool connected = outputs[VCO_OUTPUT].isConnected() | outputs[VCO_SUM_OUTPUT].isConnected();
    float gain = params[ATT_PARAM].getValue();
    int oversampling = AA::oversampling(antialias);

    double in[4][16];
    double sum[4][16];

    if (connected || expanded) {
        for (size_t ch = 0; ch < channels; ch++) {
            for (int i = 0; i < oversampling; i++) {
                float α = vco[ch].phase[i] * 2.0f * M_PI;
                float υ = sn.υ(α);

                in[i][ch] = υ;
                sum[i][ch] = vco[ch].out.sum[i] + sn.A * υ;
            }
        }

        for (size_t ch = 0; ch < channels; ch++) {
            for (int i = 0; i < oversampling; i++) {
                vco[ch].out.vco[i] = in[i][ch];
                vco[ch].out.sum[i] = sum[i][ch];
            }
        }
    }

    if (outputs[VCO_OUTPUT].isConnected()) {
        double buffer[16];
        double out[16];

        AA.out.process(antialias, in, buffer, channels);
        dcf.out.process(buffer, out, channels);

        for (size_t ch = 0; ch < channels; ch++) {
            outputs[VCO_OUTPUT].setVoltage(5.f * vco[ch].velocity * out[ch], ch);
        }

        outputs[VCO_OUTPUT].setChannels(channels);
    }

    if (outputs[VCO_SUM_OUTPUT].isConnected()) {
        double buffer[16];
        double out[16];

        AA.sum.process(antialias, sum, out, channels);
        dcf.sum.process(buffer, out, channels);

        for (size_t ch = 0; ch < channels; ch++) {
            outputs[VCO_SUM_OUTPUT].setVoltage(5.f * vco[ch].velocity * gain * out[ch], ch);
        }

        outputs[VCO_SUM_OUTPUT].setChannels(channels);
    }
}

void sn_vcox::processAUX(const ProcessArgs &args, bool expanded) {
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

void sn_vcox::recompute(const ProcessArgs &args, DCBLOCK dcblocking) {
    update.count--;
    if (update.count > 0) {
        return;
    }

    update.count = KRATE[update.krate];

    // ... antialiasing
    AA.out.recompute(args.sampleRate);
    AA.sum.recompute(args.sampleRate);

    // ... DC blocking
    dcf.out.recompute(dcblocking, args.sampleRate);
    dcf.sum.recompute(dcblocking, args.sampleRate);

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

sn_vcoxWidget::sn_vcoxWidget(sn_vcox *module) {
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
    Vec alias(middle - 5.08, top + 7 * dh - 6.35 + 1.27);

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-vcox.svg"),
                         asset::plugin(pluginInstance, "res/sn-vcox-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... parameters
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_e), module, sn_vcox::ECCENTRICITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_s), module, sn_vcox::SENSITIVITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_θ), module, sn_vcox::ROTATION_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_A), module, sn_vcox::AMPLITUDE_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δx), module, sn_vcox::DX_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input_δy), module, sn_vcox::DY_INPUT));

    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_e), module, sn_vcox::ECCENTRICITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_s), module, sn_vcox::SENSITIVITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_θ), module, sn_vcox::ROTATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_A), module, sn_vcox::AMPLITUDE_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δx), module, sn_vcox::DX_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_δy), module, sn_vcox::DY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_m), module, sn_vcox::M_PARAM));

    // ... ATT
    addParam(createParamCentered<RoundBlackKnob>(mm2px(param_att), module, sn_vcox::ATT_PARAM));

    // ... aux, VCO and VCO-Σ outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(aux), module, sn_vcox::AUX_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(vco), module, sn_vcox::VCO_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(vcox), module, sn_vcox::VCO_SUM_OUTPUT));

    // ... expander indicators
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xll), module, sn_vcox::XLL_LIGHT));
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xll), module, sn_vcox::XLR_LIGHT));
    addChild(createLightCentered<XRightLight<DarkGreenLight>>(mm2px(xrr), module, sn_vcox::XRL_LIGHT));
    addChild(createLightCentered<XLeftLight<BrightRedLight>>(mm2px(xrr), module, sn_vcox::XRR_LIGHT));
}

void sn_vcoxWidget::appendContextMenu(Menu *menu) {
    sn_vcox *module = getModule<sn_vcox>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-vcox settings"));

    menu->addChild(createIndexPtrSubmenuItem("k-rate",
                                             KRATES,
                                             &module->update.krate));

    menu->addChild(createIndexPtrSubmenuItem("AUX mode",
                                             AUX_MODES,
                                             &module->aux.mode));

    menu->addChild(createIndexPtrSubmenuItem("AUX gain",
                                             AUX_GAINS,
                                             &module->aux.gain));
}

Model *model_sn_vcox = createModel<sn_vcox, sn_vcoxWidget>("sn-vcox");
