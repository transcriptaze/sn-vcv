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
    const double taps[101];
} FIR_18kHz_4x{
    // clang-format off
    .taps = { 
       -4.70446993e-04, -2.91949771e-04,  6.11132306e-19,  3.31610501e-04,
        6.03730161e-04,  7.11082490e-04,  5.74378721e-04,  1.78727573e-04,
       -3.96977400e-04, -9.78505607e-04, -1.33549532e-03, -1.25907226e-03,
       -6.55986686e-04,  3.77746926e-04,  1.54268255e-03,  2.40453669e-03,
        2.53833544e-03,  1.70594227e-03, -2.51492230e-18, -2.11412925e-03,
       -3.89934090e-03, -4.58095088e-03, -3.64729783e-03, -1.10919181e-03,
        2.39420572e-03,  5.71577996e-03,  7.54375492e-03,  6.87611651e-03,
        3.46658796e-03, -1.93478775e-03, -7.67553876e-03, -1.16532886e-02,
       -1.20202224e-02, -7.92129461e-03,  5.74745203e-18,  9.55239877e-03,
        1.74992637e-02,  2.05266010e-02,  1.64152968e-02,  5.04856235e-03,
       -1.11091352e-02, -2.72966964e-02, -3.75183405e-02, -3.61477887e-02,
       -1.96427937e-02,  1.21381258e-02,  5.54470280e-02,  1.03198880e-01,
        1.46481428e-01,  1.76652565e-01,  1.87467727e-01,  1.76652565e-01,
        1.46481428e-01,  1.03198880e-01,  5.54470280e-02,  1.21381258e-02,
       -1.96427937e-02, -3.61477887e-02, -3.75183405e-02, -2.72966964e-02,
       -1.11091352e-02,  5.04856235e-03,  1.64152968e-02,  2.05266010e-02,
        1.74992637e-02,  9.55239877e-03,  5.74745203e-18, -7.92129461e-03,
       -1.20202224e-02, -1.16532886e-02, -7.67553876e-03, -1.93478775e-03,
        3.46658796e-03,  6.87611651e-03,  7.54375492e-03,  5.71577996e-03,
        2.39420572e-03, -1.10919181e-03, -3.64729783e-03, -4.58095088e-03,
       -3.89934090e-03, -2.11412925e-03, -2.51492230e-18,  1.70594227e-03,
        2.53833544e-03,  2.40453669e-03,  1.54268255e-03,  3.77746926e-04,
       -6.55986686e-04, -1.25907226e-03, -1.33549532e-03, -9.78505607e-04,
       -3.96977400e-04,  1.78727573e-04,  5.74378721e-04,  7.11082490e-04,
        6.03730161e-04,  3.31610501e-04,  6.11132306e-19, -2.91949771e-04,
        -4.70446993e-04,
    },
    // clang-format on
};

sn_nlm::sn_nlm() {
    INFO("sn-nlm v0.0.0");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // ... params
    configParam<LogGain>(GAIN_PARAM, -1.0f, 1.0f, 0.0f, "gain", "");

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
    for (size_t ch = 0; ch < 16; ch++) {
        antialias.in1[ch].head = 0;
        antialias.in2[ch].head = 0;

        for (size_t i = 0; i < 101; i++) {
            antialias.in1[ch].delay_line[i] = 0.0;
            antialias.in2[ch].delay_line[i] = 0.0;
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        for (size_t i = 0; i < 4; i++) {
            antialias.out[ch].x[i] = 0.f;
            antialias.out[ch].y[i] = 0.f;
        }
    }
}

json_t *sn_nlm::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "gain", json_real(gain));

    return root;
}

void sn_nlm::dataFromJson(json_t *root) {
    json_t *gain = json_object_get(root, "gain");

    if (gain) {
        float v = json_number_value(gain);

        if (v >= 0.25f && v < 4.0f) {
            this->gain = v;
        }
    }
}

void sn_nlm::process(const ProcessArgs &args) {
    gain = std::powf(2.0f, 2.0f * params[GAIN_PARAM].getValue());

    for (size_t ch = 0; ch < 1; ch++) {
        float outA = process(ch, gain);

        outputs[OUTA_OUTPUT].setVoltage(outA, ch);
    }

    processAUX(args);
}

float sn_nlm::process(int ch, float gain) {
    float in1 = inputs[IN1_INPUT].getPolyVoltage(ch);
    float in2 = inputs[IN2_INPUT].getPolyVoltage(ch);

    float in1A = filter(antialias.in1[ch], in1);
    float in2A = filter(antialias.in2[ch], in2);
    float outA = hard_limit(in1A, in2A, gain);
    filter(antialias.out[ch], outA);

    float in1B = filter(antialias.in1[ch], 0.f);
    float in2B = filter(antialias.in2[ch], 0.f);
    float outB = hard_limit(in1B, in2B, gain);
    filter(antialias.out[ch], outB);

    float in1C = filter(antialias.in1[ch], 0.f);
    float in2C = filter(antialias.in2[ch], 0.f);
    float outC = hard_limit(in1C, in2C, gain);
    filter(antialias.out[ch], outC);

    float in1D = filter(antialias.in1[ch], 0.f);
    float in2D = filter(antialias.in2[ch], 0.f);
    float outD = hard_limit(in1D, in2D, gain);

    return filter(antialias.out[ch], outD);
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
    float outY = hard_limit(outX, outX, this->gain);

    outputs[OUTX_OUTPUT].setVoltage(outX, 0);
    outputs[OUTY_OUTPUT].setVoltage(outY, 0);
}

float sn_nlm::hard_limit(float in1, float in2, float gain) {
    return clamp(gain * (in1 + in2), -5.f, 5.f);
}

float sn_nlm::filter(struct fir101 &fir, float in) {
    fir.delay_line[fir.head] = 2.f * in;

    double out = 0.0;
    size_t index = fir.head;

    for (size_t i = 0; i < 101; ++i) {
        out += FIR_18kHz_4x.taps[i] * fir.delay_line[index];

        if (index == 0) {
            index = 101 - 1;
        } else {
            --index;
        }
    }

    fir.head = (fir.head + 1) % 101;

    return 2.0 * (float)out;
}

float sn_nlm::filter(struct iir4 &iir, float in) {
    double out = 0.f;

    out = BESSEL_18kHz_4x.b[0] * in;
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
    iir.x[0] = in;

    iir.y[3] = iir.y[2];
    iir.y[2] = iir.y[1];
    iir.y[1] = iir.y[0];
    iir.y[0] = out;

    return out;
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
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_GAIN, module, sn_nlm::GAIN_PARAM));

    // ... debug
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLM_DEBUG, module, sn_nlm::DEBUG_INPUT));
}

void sn_nlmWidget::appendContextMenu(Menu *menu) {
    // sn_nlm *module = getModule<sn_nlm>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-nlm settings"));
}

} // namespace sn_nlm

Model *model_sn_nlm = createModel<sn_nlm::sn_nlm, sn_nlm::sn_nlmWidget>("sn-nlm");
