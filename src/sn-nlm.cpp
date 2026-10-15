#include "sn-nlm.hpp"

namespace sn_nlm {

const auto SN_NLM_IN1 = mm2px(Vec(7.33, 23.24));
const auto SN_NLM_IN2 = mm2px(Vec(7.33, 36.25));
const auto SN_NLM_IN3 = mm2px(Vec(7.33, 49.27));
const auto SN_NLM_IN4 = mm2px(Vec(7.33, 62.28));
const auto SN_NLM_IN5 = mm2px(Vec(7.33, 75.29));
const auto SN_NLM_IN6 = mm2px(Vec(7.33, 88.31));
const auto SN_NLM_DEBUG = mm2px(Vec(7.33, 114.34));
const auto SN_NLM_OUTX = mm2px(Vec(35.56, 75.29));
const auto SN_NLM_OUTY = mm2px(Vec(35.56, 88.31));
const auto SN_NLM_OUTA = mm2px(Vec(35.56, 114.34));
const auto SN_NLM_ATTENUATION = mm2px(Vec(35.56, 23.24));
const auto SN_NLM_GAIN = mm2px(Vec(35.56, 36.25));
const auto SN_NLM_WIDGET_CHANNELS = mm2px(Vec(35.56, 23.24));

constexpr double TWO_TO_32 = 4294967296.0; // 2^32
constexpr uint32_t MAX_RAMP = 2147483647U;

constexpr struct {
    const double a[4];
    const double b[5];
} BUTTERWORTH_18Kz_4x{
    .a = {-2.470272, 2.477971, -1.154635, 0.208848},
    .b = {0.003870, 0.015478, 0.023217, 0.015478, 0.003870},
};

constexpr struct {
    const double a[4];
    const double b[5];
} BESSEL_18kHz_4x{
    .a = {-2.319879, 2.140213, -0.918352, 0.153327},
    .b = {0.003457, 0.013827, 0.020740, 0.013827, 0.003457},
};

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

// mix funtion menu options
const std::vector<std::string> MIXERS = {
    "Linear",
    "Hard limit",
    "Soft limit",
};

sn_nlm::sn_nlm() {
    INFO("sn-nlm v0.0.0");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // ... params
    configParam(ATTENUATION_PARAM, 0.0f, 1.0f, 0.0f, "attenuation", "");
    configParam<LogGain>(GAIN_PARAM, -1.0f, 1.0f, 0.0f, "gain", "");

    getParamQuantity(ATTENUATION_PARAM)->displayPrecision = 1;
    getParamQuantity(ATTENUATION_PARAM)->randomizeEnabled = false;
    getParamQuantity(GAIN_PARAM)->randomizeEnabled = false;

    // ... inputs
    configInput(IN1_INPUT, "±5V in1");
    configInput(IN2_INPUT, "±5V in2");
    configInput(DEBUG_INPUT, "debug");

    // ... outputs
    configOutput(OUTA_OUTPUT, "outA");
    configOutput(OUTX_OUTPUT, "x");
    configOutput(OUTY_OUTPUT, "y");

    // ... initialise anti-aliasing filters
    for (size_t i = 0; i < 2; i++) {
        for (size_t ch = 0; ch < 16; ch++) {
            for (size_t j = 0; j < 25; j++) {
                source[i].fir[ch].state[i] = 0.0f;
            }
        }
    }

    for (size_t i = 0; i < 1; i++) {
        for (size_t ch = 0; ch < 16; ch++) {
            for (size_t j = 0; j < 4; j++) {
                sink[i].iir[ch].x[j] = 0.0f;
                sink[i].iir[ch].y[j] = 0.0f;
            }
        }
    }
}

json_t *sn_nlm::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "attenuation", json_real(attenuation));
    json_object_set_new(root, "gain", json_real(gain));
    json_object_set_new(root, "average", json_boolean(average));
    json_object_set_new(root, "mixer", json_integer(mixfn));

    return root;
}

void sn_nlm::dataFromJson(json_t *root) {
    json_t *attenuation = json_object_get(root, "attenuation");
    json_t *gain = json_object_get(root, "gain");
    json_t *average = json_object_get(root, "average");
    json_t *mixer = json_object_get(root, "mixer");

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

    if (average) {
        this->average = json_boolean_value(average);
    }

    if (mixer) {
        switch (json_integer_value(mixer)) {
        case LINEAR:
            this->mixfn = LINEAR;
            break;

        case HARD_LIMIT:
            this->mixfn = HARD_LIMIT;
            break;

        case SOFT_LIMIT:
            this->mixfn = SOFT_LIMIT;
            break;
        }
    }
}

void sn_nlm::process(const ProcessArgs &args) {
    attenuation = params[ATTENUATION_PARAM].getValue();
    gain = std::powf(2.0f, 2.0f * params[GAIN_PARAM].getValue());

    for (size_t ch = 0; ch < 1; ch++) {
        float out[1];

        if (average) {
            process(ch, attenuation, gain / 2.0f, out);
        } else {
            process(ch, attenuation, gain, out);
        }

        outputs[OUTA_OUTPUT].setVoltage(out[0], ch);
    }

    processAUX(args);
}

void sn_nlm::processBypass(const ProcessArgs &args) {
    for (size_t ch = 0; ch < 1; ch++) {
        outputs[OUTA_OUTPUT].setVoltage(0.0f, ch);
    }

    processAUX(args);
}

void sn_nlm::process(int ch, float attenuation, float gain, float out[1]) {
    float in[2][4];
    float buffer[1][4];

    for (size_t i = 0; i < 2; i++) {
        filter(source[i].fir[ch], (1.0f - attenuation) * inputs[source[i].input].getPolyVoltage(ch), in[i]);
    }

    for (size_t i = 0; i < 1; i++) {
        for (size_t j = 0; j < 4; j++) {
            buffer[i][j] = mix(in[0][j], in[1][j], gain);
        }
    }

    for (size_t i = 0; i < 1; i++) {
        out[i] = butterworth(sink[i].iir[ch], buffer[i]);
    }
}

void sn_nlm::processAUX(const ProcessArgs &args) {
    uint32_t tuning = static_cast<uint32_t>((20.0 / args.sampleRate) * TWO_TO_32);

    AUX.phase += tuning;
    bool msb = (AUX.phase >> 31) & 1;
    uint32_t ramp = AUX.phase & 0x7FFFFFFF;
    if (msb) {
        ramp = MAX_RAMP - ramp;
    }

    double normalized = (static_cast<double>(ramp) / static_cast<double>(MAX_RAMP)) * 2.0 - 1.0;
    float outX = normalized * 5.0f;
    float outY = mix(outX, outX, gain);

    outputs[OUTX_OUTPUT].setVoltage(outX, 0);
    outputs[OUTY_OUTPUT].setVoltage(outY, 0);
}

float sn_nlm::mix(float in1, float in2, float gain) {
    switch (mixfn) {
    case HARD_LIMIT:
        return hard_limit(in1, in2, this->gain);

    case SOFT_LIMIT:
        return soft_limit(in1, in2, this->gain);

    default:
        return linear(in1, in2, this->gain);
    }
}

float sn_nlm::linear(float in1, float in2, float gain) {
    return gain * (in1 + in2);
}

float sn_nlm::hard_limit(float in1, float in2, float gain) {
    return clamp(gain * (in1 + in2), -5.0f, 5.0f);
}

float sn_nlm::soft_limit(float in1, float in2, float gain) {
    float v = gain * (in1 + in2);

    return 5.0f * std::tanh(v / 5.0f);
}

// cubic soft-clipping: f(x) = x - (x^3)/3
float sn_nlm::soft_limit_cubic(float in1, float in2, float gain) {
    float v = (gain * (in1 + in2)) / 5.0f; // normalize to -1..1 range

    if (v <= -1.0f) {
        return -5.0f;
    }

    if (v >= 1.0f) {
        return 5.0f;
    }

    float shaped = v - (v * v * v) / 3.0f;

    return shaped * 7.5f;
}

// void sn_nlm::filter(struct fir101 &fir, float in, float out[4]) {
//     float _in[4] = {in, 0.0f, 0.0f, 0.0f};
//
//     for (size_t i = 0; i < 4; i++) {
//         fir.delay_line[fir.head] = _in[i];
//
//         double acc = 0.0;
//         size_t index = fir.head;
//
//         for (size_t j = 0; j < 101; ++j) {
//             acc += FIR_18kHz_4x.taps[j] * fir.delay_line[index];
//
//             if (index == 0) {
//                 index = 101 - 1;
//             } else {
//                 --index;
//             }
//         }
//
//         fir.head = (fir.head + 1) % 101;
//
//         out[i] = 4.0 * (float)acc;
//     }
// }

void sn_nlm::filter(struct polyphase100 &polyphase, float in, float out[4]) {
    for (size_t i = 24; i > 0; --i) {
        polyphase.state[i] = polyphase.state[i - 1];
    }
    polyphase.state[0] = in;

    for (size_t p = 0; p < 4; ++p) {
        double acc = 0.0;
        for (size_t i = 0; i < 25; ++i) {
            acc += polyphase.state[i] * POLYPHASE_18kHz_4x.taps[p][i];
        }

        out[p] = 4.0f * acc;
    }
}

float sn_nlm::butterworth(struct iir4 &iir, float in[4]) {
    for (size_t i = 0; i < 4; i++) {
        double out = 0.0f;

        out = BUTTERWORTH_18Kz_4x.b[0] * in[i];
        out += BUTTERWORTH_18Kz_4x.b[1] * iir.x[0];
        out += BUTTERWORTH_18Kz_4x.b[2] * iir.x[1];
        out += BUTTERWORTH_18Kz_4x.b[3] * iir.x[2];
        out += BUTTERWORTH_18Kz_4x.b[4] * iir.x[3];

        out -= BUTTERWORTH_18Kz_4x.a[0] * iir.y[0];
        out -= BUTTERWORTH_18Kz_4x.a[1] * iir.y[1];
        out -= BUTTERWORTH_18Kz_4x.a[2] * iir.y[2];
        out -= BUTTERWORTH_18Kz_4x.a[3] * iir.y[3];

        iir.x[3] = iir.x[2];
        iir.x[2] = iir.x[1];
        iir.x[1] = iir.x[0];
        iir.x[0] = in[i];

        iir.y[3] = iir.y[2];
        iir.y[2] = iir.y[1];
        iir.y[1] = iir.y[0];
        iir.y[0] = out;
    }

    return iir.y[0];
}

float sn_nlm::bessel(struct iir4 &iir, float in[4]) {
    for (size_t i = 0; i < 4; i++) {
        double out = 0.0f;

        out = BESSEL_18kHz_4x.b[0] * in[i];
        out += BESSEL_18kHz_4x.b[1] * iir.x[0];
        out += BESSEL_18kHz_4x.b[2] * iir.x[1];
        out += BESSEL_18kHz_4x.b[3] * iir.x[2];
        out += BESSEL_18kHz_4x.b[4] * iir.x[3];

        out -= BESSEL_18kHz_4x.a[0] * iir.y[0];
        out -= BESSEL_18kHz_4x.a[1] * iir.y[1];
        out -= BESSEL_18kHz_4x.a[2] * iir.y[2];
        out -= BESSEL_18kHz_4x.a[3] * iir.y[3];

        iir.x[3] = iir.x[2];
        iir.x[2] = iir.x[1];
        iir.x[1] = iir.x[0];
        iir.x[0] = in[i];

        iir.y[3] = iir.y[2];
        iir.y[2] = iir.y[1];
        iir.y[1] = iir.y[0];
        iir.y[0] = out;
    }

    return iir.y[0];
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
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_IN1, module, sn_nlm::IN1_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_IN2, module, sn_nlm::IN2_INPUT));

    // ... outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_NLM_OUTA, module, sn_nlm::OUTA_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_NLM_OUTX, module, sn_nlm::OUTX_OUTPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_NLM_OUTY, module, sn_nlm::OUTY_OUTPUT));

    // ... parameters
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_ATTENUATION, module, sn_nlm::ATTENUATION_PARAM));
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_GAIN, module, sn_nlm::GAIN_PARAM));

    // ... debug
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_DEBUG, module, sn_nlm::DEBUG_INPUT));
}

void sn_nlmWidget::appendContextMenu(Menu *menu) {
    sn_nlm *module = getModule<sn_nlm>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-nlm settings"));

    menu->addChild(createBoolPtrMenuItem("Average", "", &module->average));

    menu->addChild(createIndexPtrSubmenuItem("Mix function",
                                             MIXERS,
                                             &module->mixfn));
}

} // namespace sn_nlm

Model *model_sn_nlm = createModel<sn_nlm::sn_nlm, sn_nlm::sn_nlmWidget>("sn-nlm");
