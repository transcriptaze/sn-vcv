#include "plugin.hpp"

namespace sn_nlm {

struct iir4 {
    double x[4];
    double y[4];
};

struct fir101 {
    size_t head;
    double delay_line[101];
};

struct polyphase100 {
    float state[25];
};

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
    void processBypass(const ProcessArgs &args) override;

    float process(int channel, float gain);
    void processAUX(const ProcessArgs &args);

    float hard_limit(float in1, float in2, float gain);
    void filter(struct fir101 &fir, float in, float out[4]);
    void filter(struct polyphase100 &polyphase, float in, float out[4]);

    float butterworth(struct iir4 &iir, float in[4]);
    float bessel(struct iir4 &iir, float in[4]);

    float gain = 1.0;

    // ... antialiasing
    struct {
        struct fir101 fir[2][16];
        struct polyphase100 polyphase[2][16];
        struct iir4 out[16];
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

} // namespace sn_nlm
