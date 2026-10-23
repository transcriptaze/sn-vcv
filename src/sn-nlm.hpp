#include "plugin.hpp"

namespace sn_nlm {

struct iir4 {
    double x[4];
    double y[4];
};

struct polyphase100 {
    float state[25];
};

struct hysteresis {
    double prev_h_;
    double current_b_;
    bool increasing_;
};

struct source {
    struct polyphase100 fir[16];
    float buffer[4][16];
};

struct mixer {
    struct hysteresis hysteresis[16];
};

struct sink {
    struct iir4 iir[16];
    float buffer[16][4];
};

struct sn_nlm : Module {
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

    enum MIXFN {
        LINEAR,
        HARD_LIMIT,
        SOFT_LIMIT,
        EXPAND,
        HYSTERESIS,
    };

    constexpr static int INPUTS[] = {IN1_INPUT, IN2_INPUT};
    constexpr static int N_INPUTS = sizeof(INPUTS) / sizeof(int);
    constexpr static int N_MIXERS = 1;
    constexpr static int N_OUTPUTS = 1;
    constexpr static int CHANNELS = 1;

    sn_nlm();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void process(const ProcessArgs &args) override;
    void processBypass(const ProcessArgs &args) override;
    void processAUX(const ProcessArgs &args);

    void mix2x4(source &p, source &q, mixer &r, sink &s, float gain);
    float mix(float in1, float in2, float gain, struct hysteresis &h);
    void mix(float in1[4], float in2[4], float out[4], float gain, struct hysteresis &h);

    float linear(float in1, float in2, float gain);
    float hard_limit(float in1, float in2, float gain);
    float soft_limit(float in1, float in2, float gain);
    float soft_limit_cubic(float in1, float in2, float gain);
    float expand(float in1, float in2, float gain);
    float hysteresis(float in1, float in2, float gain, struct hysteresis &h);

    void filter(struct polyphase100 &polyphase, float in, float out[4]);
    float butterworth(struct iir4 &iir, float in[4]);
    float bessel(struct iir4 &iir, float in[4]);

    // ... parameters
    float attenuation = 0.0f;
    float gain = 1.0f;
    bool average = false;
    MIXFN mixfn = SOFT_LIMIT;

    // ... groups
    struct source sources[N_INPUTS];
    struct mixer mixers[N_MIXERS];
    struct sink sinks[N_OUTPUTS];

    // ... AUX
    struct {
        uint32_t phase;
        struct hysteresis hysteresis;
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

    void setDisplayValue(float v) override {
        float clamped = clamp(v, 0.25f, 4.0f);
        float gain = log2(clamped) / 2.0f;

        setValue(gain);
    }
};

} // namespace sn_nlm
