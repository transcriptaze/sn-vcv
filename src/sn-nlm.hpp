#include "plugin.hpp"
#include "sn.hpp"

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

struct mixer {
    struct polyphase100 interpolator[16];
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
        SNX,
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

    void mix2x4(InputId p, InputId q, mixer &r, sink &s);

    float mix(float in, struct hysteresis &h);

    float linear(float in);
    float hard_limit(float in);
    float soft_limit(float in);
    float soft_limit_cubic(float in);
    float expand(float in);
    float hysteresis(float in, struct hysteresis &h);
    float sn(float in);

    void filter(struct polyphase100 &polyphase, float in, float out[4]);
    float butterworth(struct iir4 &iir, float in[4]);

    // ... parameters
    float attenuation = 0.0f;
    float gain = 1.0f;
    bool average = false;
    MIXFN mixfn = SOFT_LIMIT;

    // ... mix groups
    struct mixer mixers[N_MIXERS];
    struct sink sinks[N_OUTPUTS];

    // ... AUX
    struct {
        uint32_t phase;
        struct hysteresis hysteresis;
    } AUX{
        .phase = 0,
    };

    // ... sn
    struct SN _sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

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
