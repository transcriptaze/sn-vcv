#include "dc-blocking/DCF.hpp"
#include "plugin.hpp"
#include "sn.hpp"

namespace sn_nlm {

struct polyphase100 {
    float interpolator[25];
    float decimator[4][25];
    int head = 0;
};

struct sn_nlm : Module {
    constexpr static int CHANNELS = 16;
    constexpr static double TWO_TO_32 = 4294967296.0; // 2^32
    constexpr static uint32_t MAX_RAMP = 2147483647U;

    enum ParamId {
        ECCENTRICITY_PARAM,
        SENSITIVITY_PARAM,
        ROTATION_PARAM,
        AMPLITUDE_PARAM, // NTS: placeholder - not used
        DX_PARAM,
        DY_PARAM,
        PHI_PARAM,
        M_PARAM,
        ATTENUATION_PARAM,
        GAIN_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        IN_INPUT,

        ECCENTRICITY_INPUT,
        SENSITIVITY_INPUT,
        ROTATION_INPUT,
        AMPLITUDE_INPUT, // NTS: placeholder - not used
        DX_INPUT,
        DY_INPUT,
        PHI_INPUT,
        ATTENUATION_INPUT, // placeholder (future)
        GAIN_INPUT,        // placeholder (future)
        DEBUG_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        OUT_OUTPUT,
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
    void processIn(const ProcessArgs &args);
    void processAUX(const ProcessArgs &args);

    void interpolate(struct polyphase100 &polyphase, float in, float out[4]);
    float decimate(struct polyphase100 &polyphase, float in[4]);
    float mix(float in);
    void recompute();

    // ... parameters
    float attenuation = 0.0f;
    float gain = 1.0f;
    int dcBlocking = 1;

    // ... mixer
    struct {
        struct polyphase100 filter[CHANNELS];
        struct DCF dcf;
    } mixer;

    // ... sn
    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct {
        int krate;
        int count;
    } update{
        .krate = 2,
        .count = 0,
    };

    // ... AUX
    struct {
        uint32_t phase;
    } AUX{
        .phase = 0,
    };
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
