#include "sn-nlm.hpp"

namespace sn_nlm {

const auto SN_NLM_INPUT_IN = mm2px(Vec(7.33, 23.24));
const auto SN_NLM_INPUT_E = mm2px(Vec(24.13, 23.24));
const auto SN_NLM_INPUT_S = mm2px(Vec(24.13, 36.25));
const auto SN_NLM_INPUT_THETA = mm2px(Vec(24.13, 49.27));
const auto SN_NLM_INPUT_DX = mm2px(Vec(24.13, 62.28));
const auto SN_NLM_INPUT_DY = mm2px(Vec(24.13, 75.29));
const auto SN_NLM_INPUT_PHI = mm2px(Vec(24.13, 88.31));
const auto SN_NLM_INPUT_DEBUG = mm2px(Vec(7.33, 7.62));

const auto SN_NLM_OUTPUT_OUT = mm2px(Vec(57.12, 62.28));
const auto SN_NLM_OUTPUT_X = mm2px(Vec(44.42, 114.34));
const auto SN_NLM_OUTPUT_Y = mm2px(Vec(57.12, 114.34));

const auto SN_NLM_PARAM_E = mm2px(Vec(38.07, 23.24));
const auto SN_NLM_PARAM_S = mm2px(Vec(38.07, 36.25));
const auto SN_NLM_PARAM_THETA = mm2px(Vec(38.07, 49.27));
const auto SN_NLM_PARAM_DX = mm2px(Vec(38.07, 62.28));
const auto SN_NLM_PARAM_DY = mm2px(Vec(38.07, 75.29));
const auto SN_NLM_PARAM_PHI = mm2px(Vec(38.07, 88.31));
const auto SN_NLM_PARAM_M = mm2px(Vec(38.07, 101.32));
const auto SN_NLM_PARAM_ATTENUATION = mm2px(Vec(55.85, 23.24));
const auto SN_NLM_PARAM_GAIN = mm2px(Vec(55.85, 42.76));

constexpr struct {
    const double taps[4][25];
} POLYPHASE_18kHz_4x{
    // clang-format off
    .taps = {
        {
            -3.97413335e-04,  6.59195838e-04, -6.71203177e-04, -1.72181350e-04,
             2.18356030e-03, -4.32920111e-03,  4.10029020e-03,  9.19749721e-04,
            -1.04580445e-02,  1.97269540e-02, -1.95196199e-02, -5.51179954e-03,
             1.63591542e-01,  1.25898134e-01, -2.98487457e-02, -2.67726530e-03,
             1.38819673e-02, -1.22817019e-02,  5.43046798e-03,  5.75373833e-04,
            -3.03006506e-03,  2.50666309e-03, -9.83440823e-04, -9.14515086e-05,
             4.67012328e-04,
        },
        {
            -1.54078989e-04,  6.49144451e-04, -1.15016433e-03,  9.36230975e-04,
             9.17036077e-04, -4.24804639e-03,  6.80009830e-03, -4.84665351e-03,
            -4.30926472e-03,  1.93615905e-02, -3.34732299e-02,  3.27022539e-02,
             1.84640003e-01,  7.92496905e-02, -3.85123472e-02,  1.15115548e-02,
             4.73504725e-03, -9.93345919e-03,  7.46714654e-03, -2.50899043e-03,
            -1.02281745e-03,  1.97523848e-03, -1.30684360e-03,  3.89366620e-04,
             1.62716772e-04,
        },
        {
             1.62716772e-04,  3.89366620e-04, -1.30684360e-03,  1.97523848e-03,
            -1.02281745e-03, -2.50899043e-03,  7.46714654e-03, -9.93345919e-03,
             4.73504725e-03,  1.15115548e-02, -3.85123472e-02,  7.92496905e-02,
             1.84640003e-01,  3.27022539e-02, -3.34732299e-02,  1.93615905e-02,
            -4.30926472e-03, -4.84665351e-03,  6.80009830e-03, -4.24804639e-03,
             9.17036077e-04,  9.36230975e-04, -1.15016433e-03,  6.49144451e-04,
            -1.54078989e-04,
        },
        {
             4.67012328e-04, -9.14515086e-05, -9.83440823e-04,  2.50666309e-03,
            -3.03006506e-03,  5.75373833e-04,  5.43046798e-03, -1.22817019e-02,
             1.38819673e-02, -2.67726530e-03, -2.98487457e-02,  1.25898134e-01,
             1.63591542e-01, -5.51179954e-03, -1.95196199e-02,  1.97269540e-02,
            -1.04580445e-02,  9.19749721e-04,  4.10029020e-03, -4.32920111e-03,
             2.18356030e-03, -1.72181350e-04, -6.71203177e-04,  6.59195838e-04,
            -3.97413335e-04,
        },
    },
    // clang-format on
};

std::vector<std::string> DCBLOCKING = {
    "Off",
    "On",
};

sn_nlm::sn_nlm() {
    INFO("sn-nlm v0.0.0");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // ... params

    configParam(ECCENTRICITY_PARAM, -1.0f, +1.0f, 0.0f, "ε");
    configParam(SENSITIVITY_PARAM, 0.0f, +5.0f, 2.5f, "s");
    configParam(ROTATION_PARAM, -90.0f, +90.0f, 0.0f, "θ");
    configParam(AMPLITUDE_PARAM, 0.0f, +1.0f, 1.0f, "a");
    configParam(DX_PARAM, -1.0f, +1.0f, 0.0f, "δx");
    configParam(DY_PARAM, -1.0f, +1.0f, 0.0f, "δy");
    configParam(PHI_PARAM, -90.0f, +90.0f, 0.0f, "φ");
    configSwitch(M_PARAM, 1.f, 5.f, 1.f, "m", {"1", "2", "3", "4", "5"});

    configParam(ATTENUATION_PARAM, 0.0f, 1.0f, 0.0f, "attenuation", "");
    configParam<LogGain>(GAIN_PARAM, -1.0f, 1.0f, 0.0f, "gain", "");

    getParamQuantity(ATTENUATION_PARAM)->displayPrecision = 1;

    getParamQuantity(ATTENUATION_PARAM)->randomizeEnabled = false;
    getParamQuantity(GAIN_PARAM)->randomizeEnabled = false;
    getParamQuantity(M_PARAM)->randomizeEnabled = false;

    // ... inputs
    configInput(IN_INPUT, "±10V in");

    // ... outputs
    configOutput(OUT_OUTPUT, "±5V out");
    configOutput(OUTX_OUTPUT, "x");
    configOutput(OUTY_OUTPUT, "y");

    // ... initialise mixers
    for (int ch = 0; ch < CHANNELS; ch++) {
        for (int j = 0; j < 25; j++) {
            mixer.filter[ch].interpolator[j] = 0.0f;
        }
    }

    for (int ch = 0; ch < CHANNELS; ch++) {
        mixer.filter[ch].head = 0;

        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 25; k++) {
                mixer.filter[ch].decimator[j][k] = 0.0f;
            }
        }
    }

    mixer.dcf = DCF(DCBLOCK_MEDIUM);

    // ... sn
    float e = -0.8f;
    float s = 1.0f;
    float θ = 0.f;
    float a = 1.0f;
    float δx = 0.f;
    float δy = 0.f;
    float m = 1.0f;

    sn.ε = std::tanh(s * e);
    sn.θ = θ * M_PI / 180.0f;
    sn.A = clamp(a, 0.0f, +1.0f);
    sn.δx = clamp(δx, -1.0f, +1.0f);
    sn.δy = clamp(δy, -1.0f, +1.0f);
    sn.m = m;

    sn.recompute();
}

json_t *sn_nlm::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "attenuation", json_real(attenuation));
    json_object_set_new(root, "gain", json_real(gain));
    json_object_set_new(root, "k-rate", json_integer(update.krate));

    switch (dcBlocking) {
    case 0:
        json_object_set_new(root, "DC-blocking", json_string("none"));
        break;

    case 1:
        json_object_set_new(root, "DC-blocking", json_string("medium"));
        break;
    }

    return root;
}

void sn_nlm::dataFromJson(json_t *root) {
    json_t *attenuation = json_object_get(root, "attenuation");
    json_t *gain = json_object_get(root, "gain");
    json_t *krate = json_object_get(root, "k-rate");
    json_t *dcBlocking = json_object_get(root, "DC-blocking");

    if (attenuation) {
        float v = json_number_value(attenuation);
        if (v >= 0.0f && v < 1.0f) {
            this->attenuation = v;
        }
    }

    if (gain) {
        float v = json_number_value(gain);
        if (v >= 0.25f && v < 4.0f) {
            this->gain = v;
        }
    }

    if (krate) {
        int v = json_integer_value(krate);

        if (v >= 0 && v < 4) {
            update.krate = v;
        }
    }

    if (dcBlocking) {
        std::string s(json_string_value(dcBlocking));

        if (s == "none") {
            this->dcBlocking = 0;
        } else {
            this->dcBlocking = 1;
        }
    }
}

void sn_nlm::process(const ProcessArgs &args) {
    attenuation = params[ATTENUATION_PARAM].getValue();
    gain = std::powf(2.0f, 2.0f * params[GAIN_PARAM].getValue());

    recompute();
    processIn(args);
    processAUX(args);
}

void sn_nlm::processIn(const ProcessArgs &args) {
    if (!outputs[OUT_OUTPUT].isConnected()) {
        return;
    }

    if (!inputs[IN_INPUT].isConnected()) {
        outputs[OUT_OUTPUT].setChannels(1);
        outputs[OUT_OUTPUT].setVoltage(0.f);
        return;
    }

    int channels = inputs[IN_INPUT].getChannels() > 0 ? inputs[IN_INPUT].getChannels() : 1;
    float k = 1.0f - attenuation;
    float g = k * gain;

    for (int ch = 0; ch < channels; ch++) {
        float in = inputs[IN_INPUT].getPolyVoltage(ch);
        float buffer[4];

        interpolate(mixer.filter[ch], g * in, buffer);

        float mixed[4];
        for (int i = 0; i < 4; i++) {
            mixed[i] = mix(buffer[i]);
        }

        float out = decimate(mixer.filter[ch], mixed);

        if (dcBlocking) {
            out = mixer.dcf.process(out, ch);
        }

        outputs[OUT_OUTPUT].setVoltage(out, ch);
    }

    outputs[OUT_OUTPUT].setChannels(channels);
}

void sn_nlm::processAUX(const ProcessArgs &args) {
    uint32_t tuning = static_cast<uint32_t>((20.0 / args.sampleRate) * TWO_TO_32);

    AUX.phase += tuning;
    bool msb = (AUX.phase >> 31) & 1;
    uint32_t ramp = AUX.phase & 0x7FFFFFFF;
    if (msb) {
        ramp = MAX_RAMP - ramp;
    }

    float k = 1.0f - attenuation;
    float g = k * gain;

    double normalized = (static_cast<double>(ramp) / static_cast<double>(MAX_RAMP)) * 2.0 - 1.0;
    float outX = normalized * 5.0f;
    float outY = mix(g * (outX + outX));

    outputs[OUTX_OUTPUT].setVoltage(outX, 0);
    outputs[OUTY_OUTPUT].setVoltage(outY, 0);
}

float sn_nlm::mix(float in) {
    float α = clamp(in / 5.0f, -1.0f, +1.0f);
    float αʼ = 0.5f * M_PI * α;
    float υ = sn.υ(αʼ);

    return 5.0f * υ;
}

void sn_nlm::interpolate(struct polyphase100 &filter, float in, float out[4]) {
    for (size_t i = 24; i > 0; --i) {
        filter.interpolator[i] = filter.interpolator[i - 1];
    }

    filter.interpolator[0] = in;

    for (size_t p = 0; p < 4; ++p) {
        double acc = 0.0;
        for (size_t i = 0; i < 25; ++i) {
            acc += filter.interpolator[i] * POLYPHASE_18kHz_4x.taps[p][i];
        }

        out[p] = 4.0f * acc;
    }
}

float sn_nlm::decimate(struct polyphase100 &filter, float in[4]) {
    filter.head = (filter.head + 1) % 25;

    filter.decimator[0][filter.head] = in[3];
    filter.decimator[1][filter.head] = in[2];
    filter.decimator[2][filter.head] = in[1];
    filter.decimator[3][filter.head] = in[0];

    float out[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < 25; i++) {
        int ix = (filter.head + 25 - i) % 25;

        out[0] += POLYPHASE_18kHz_4x.taps[0][i] * filter.decimator[0][ix];
        out[1] += POLYPHASE_18kHz_4x.taps[1][i] * filter.decimator[1][ix];
        out[2] += POLYPHASE_18kHz_4x.taps[2][i] * filter.decimator[2][ix];
        out[3] += POLYPHASE_18kHz_4x.taps[3][i] * filter.decimator[3][ix];
    }

    return out[0] + out[1] + out[2] + out[3];
}

void sn_nlm::recompute() {
    // ... update parameters?
    if (--update.count > 0) {
        return;
    }

    update.count = KRATE[update.krate];

    // ... param values
    float e = params[ECCENTRICITY_PARAM].getValue();
    float s = params[SENSITIVITY_PARAM].getValue();
    float θ = params[ROTATION_PARAM].getValue();
    float δx = params[DX_PARAM].getValue();
    float δy = params[DY_PARAM].getValue();
    float φ = params[PHI_PARAM].getValue();
    float m = params[M_PARAM].getValue();

    // ... modulate with inputs
    e += clamp(inputs[ECCENTRICITY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    s += 5.0f * clamp(inputs[SENSITIVITY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    θ += 90.0f * clamp(inputs[ROTATION_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    δx += clamp(inputs[DX_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    δy += clamp(inputs[DY_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);
    φ += 90.0f * clamp(inputs[PHI_INPUT].getVoltage() / 5.0f, -1.0f, +1.0f);

    // ... preconditioning
    e = clamp(e, -1.0f, +1.0f);
    s = clamp(s, 0.0f, +5.0f);
    θ = clamp(θ, -89.95f, +89.95f);
    φ = clamp(φ, -89.95f, +89.95f);

    // ... recalculate SN parameters
    sn.ε = std::tanh(s * e);
    sn.θ = θ * M_PI / 180.0f;
    sn.A = 1.0f;
    sn.δx = clamp(δx, -0.99f, +0.99f);
    sn.δy = clamp(δy, -0.99f, +0.99f);
    sn.Φ = φ * M_PI / 180.0f;
    sn.m = m;

    sn.recompute();
}

sn_nlmWidget::sn_nlmWidget(sn_nlm *module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-nlm.svg"),
                         asset::plugin(pluginInstance, "res/sn-nlm-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... inputs
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_IN, module, sn_nlm::IN_INPUT));

    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_E, module, sn_nlm::ECCENTRICITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_S, module, sn_nlm::SENSITIVITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_THETA, module, sn_nlm::ROTATION_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_DX, module, sn_nlm::DX_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_DY, module, sn_nlm::DY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_INPUT_PHI, module, sn_nlm::PHI_INPUT));

    // ... outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_NLM_OUTPUT_OUT, module, sn_nlm::OUT_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_NLM_OUTPUT_X, module, sn_nlm::OUTX_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_NLM_OUTPUT_Y, module, sn_nlm::OUTY_OUTPUT));

    // ... parameters
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_E, module, sn_nlm::ECCENTRICITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_S, module, sn_nlm::SENSITIVITY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_THETA, module, sn_nlm::ROTATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_DX, module, sn_nlm::DX_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_DY, module, sn_nlm::DY_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_PHI, module, sn_nlm::PHI_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_M, module, sn_nlm::M_PARAM));

    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_ATTENUATION, module, sn_nlm::ATTENUATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_GAIN, module, sn_nlm::GAIN_PARAM));
}

void sn_nlmWidget::appendContextMenu(Menu *menu) {
    sn_nlm *module = getModule<sn_nlm>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-nlm settings"));
    menu->addChild(createIndexPtrSubmenuItem("k-rate", KRATES, &module->update.krate));
    menu->addChild(createIndexPtrSubmenuItem("DC blocking", DCBLOCKING, &module->dcBlocking));
}

} // namespace sn_nlm

Model *model_sn_nlm = createModel<sn_nlm::sn_nlm, sn_nlm::sn_nlmWidget>("sn-nlm");
