#include "plugin.hpp"

namespace sn_nlm {

struct iir4 {
    double x[4];
    double y[4];
};

struct polyphase100 {
    float state[25];
};

struct sn_nlm : Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum ParamId {
        ATTENUATION_PARAM,
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
    void processBypass(const ProcessArgs &args) override;

    void process(int channel, float attenuation, float gain, float out[1]);
    void processAUX(const ProcessArgs &args);

    float hard_limit(float in1, float in2, float gain);
    void filter(struct polyphase100 &polyphase, float in, float out[4]);

    float butterworth(struct iir4 &iir, float in[4]);
    float bessel(struct iir4 &iir, float in[4]);

    float attenuation = 0.f;
    float gain = 1.f;
    bool average = false;

    // ... groups
    struct {
        int input;
        struct polyphase100 fir[16];
    } source[2] = {
        {.input = IN1_INPUT},
        {.input = IN2_INPUT},
    };

    struct {
        int output;
        struct iir4 iir[16];
    } sink[1] = {
        {.output = OUTA_OUTPUT},
    };

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

} // namespace sn_nlm
