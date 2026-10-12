#include "plugin.hpp"

namespace sn_nlm {

struct IIR4 {
    const double a[4];
    const double b[5];
};

struct FIR101 {
    const double taps[101];
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

    float hard_limit(float in1, float in2, float gain);
    float filter(const IIR4 &iir, float in);
    float filter(const FIR101 &fir, float in);

    float gain = 1.0;

    // ... antialiasing
    struct {
        double x[4];
        double y[4];
    } iir;

    struct {
        size_t head;
        double delay_line[101];
    } fir;

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
