#include "plugin.hpp"
#include "sn.hpp"

#include "antialias/AA.hpp"
#include "dc-blocking/DCF.hpp"
#include "widgets/xlight.hpp"

struct sn_sigma : Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum ParamId {
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

    sn_sigma();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void process(const ProcessArgs &args) override;

    void recompute(const ProcessArgs &args, size_t);

    int channels();

    // ... debug
    dsp::SchmittTrigger debug;
};

struct sn_sigmaWidget : ModuleWidget {
    sn_sigmaWidget(sn_sigma *);

    void appendContextMenu(Menu *) override;
};
