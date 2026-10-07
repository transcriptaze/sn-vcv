#include "plugin.hpp"

const auto SN_NLM_INPUT_IN1 = mm2px(Vec(7.33, 23.24));
const auto SN_NLM_INPUT_IN2 = mm2px(Vec(7.33, 36.25));
const auto SN_NLM_INPUT_IN3 = mm2px(Vec(7.33, 49.27));
const auto SN_NLM_INPUT_IN4 = mm2px(Vec(7.33, 62.28));
const auto SN_NLM_INPUT_IN5 = mm2px(Vec(7.33, 75.29));
const auto SN_NLM_INPUT_IN6 = mm2px(Vec(7.33, 88.31));
const auto SN_NLM_INPUT_DEBUG = mm2px(Vec(7.33, 114.34));
const auto SN_NLM_OUTUT_X = mm2px(Vec(35.56, 75.29));
const auto SN_NLM_OUTUT_Y = mm2px(Vec(35.56, 88.31));
const auto SN_NLM_OUTUT_OUT = mm2px(Vec(35.56, 114.34));
const auto SN_NLM_PARAM_GAIN = mm2px(Vec(35.56, 36.25));

const auto SN_NLM_WIDGET_CHANNELS = mm2px(Vec(35.56, 23.24));

struct sn_nlm : Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum ParamId {
        GAIN_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        IN1_INPUT,
        IN2_INPUT,
        DEBUG_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        OUTA_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        LIGHTS_LEN
    };

    sn_nlm();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void process(const ProcessArgs &args) override;

    void recompute(const ProcessArgs &args, size_t);

    float hard_max(float in1, float in2, float gain);

    float gain = 1.0;

    // ... debug
    dsp::SchmittTrigger debug;
};

struct sn_nlmWidget : ModuleWidget {
    sn_nlmWidget(sn_nlm *);

    void appendContextMenu(Menu *) override;
};

struct LogGain : ParamQuantity {
    float getDisplayValue() override {
        sn_nlm *module = reinterpret_cast<sn_nlm *>(this->module);

        return module->gain;
    }
};
