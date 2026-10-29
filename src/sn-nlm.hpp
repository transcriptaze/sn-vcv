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

struct mixer {
    struct polyphase100 interpolator[16];
};

struct sink {
    struct iir4 iir[16];
    float buffer[16][4];
};

struct sn_nlm : Module {
    enum ParamId {
        ECCENTRICITY_PARAM,
        SENSITIVITY_PARAM,
        ROTATION_PARAM,
        AMPLITUDE_PARAM,
        DX_PARAM,
        DY_PARAM,
        PHI_PARAM,
        M_PARAM,
        ATTENUATION_PARAM,
        GAIN_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        IN1_INPUT,
        IN2_INPUT,
        IN3_INPUT,
        IN4_INPUT,
        IN5_INPUT,
        IN6_INPUT,
        IN7_INPUT,
        IN8_INPUT,

        ECCENTRICITY_INPUT,
        SENSITIVITY_INPUT,
        ROTATION_INPUT,
        AMPLITUDE_INPUT,
        DX_INPUT,
        DY_INPUT,
        PHI_INPUT,

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
    float mix(float in);

    void filter(struct polyphase100 &polyphase, float in, float out[4]);
    float butterworth(struct iir4 &iir, float in[4]);

    // ... parameters
    float attenuation = 0.0f;
    float gain = 1.0f;
    bool average = false;

    // ... mix groups
    struct mixer mixers[N_MIXERS];
    struct sink sinks[N_OUTPUTS];

    // ... AUX
    struct {
        uint32_t phase;
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
