#include "sn-nlm.hpp"

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
const auto SN_NLM_GAIN = mm2px(Vec(35.56, 36.25));

const auto SN_NLM_CHANNELS = mm2px(Vec(35.56, 23.24));

constexpr double TWO_TO_32 = 4294967296.0; // 2^32
constexpr uint32_t MAX_RAMP = 2147483647U;

// 18.0kHz @48kHz
constexpr struct {
    const double a[4];
    const double b[5];
} IIR{
    .a = {-3.231676, 3.976641, -2.201366, 0.461665},
    .b = {0.000329, 0.001316, 0.001974, 0.001316, 0.000329},
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
    for (size_t i = 0; i < 16; i++) {
        antialias.in[i].setCoefficients(IIR.b, IIR.a);
    }

    for (size_t i = 0; i < 16; i++) {
        antialias.in[i].reset();
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
    float in1 = inputs[IN1_INPUT].getPolyVoltage(0);
    float in2 = inputs[IN2_INPUT].getPolyVoltage(0);
    float gain = params[GAIN_PARAM].getValue();

    this->gain = std::powf(2.0f, 2.0f * gain);

    float outA = hard_limit(in1, in2, this->gain);

    outputs[OUTA_OUTPUT].setVoltage(outA, 0);

    // ... AUX
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
    // ... zero stuffing
    antialias.in[0].process(in1);
    antialias.in[0].process(0.f);
    antialias.in[0].process(0.f);

    return 8.0 * antialias.in[0].process(0.f);

    // return clamp(gain * (in1 + in2), -5.f, 5.f);
}

sn_nlmWidget::sn_nlmWidget(sn_nlm *module) {
    // float left = 7.331;
    // // float middle = 20.351;
    // float right = 35.56;
    // float top = 21.968 + 1.27;
    // float dh = 13.014;

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

Model *model_sn_nlm = createModel<sn_nlm, sn_nlmWidget>("sn-nlm");
