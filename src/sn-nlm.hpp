#include "plugin.hpp"

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
        OUTX_OUTPUT,
        OUTY_OUTPUT,
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

    float hard_limit(float in1, float in2, float gain);

    float gain = 1.0;

    // ... antialiasing
    struct {
        dsp::IIRFilter<5, 5, double> in[16];
    } antialias;

    // ... AUX
    struct {
        uint32_t phase;
    } AUX{
        .phase = 0,
    };

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
