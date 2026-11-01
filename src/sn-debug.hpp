#include "plugin.hpp"

struct sn_debug : Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum ParamId {
        PARAMS_LEN
    };

    enum InputId {
        PITCH_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        VCO_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        LIGHTS_LEN
    };

    sn_debug();

    void process(const ProcessArgs &args) override;

    uint32_t α = 0;
};

struct sn_debugWidget : ModuleWidget {
    sn_debugWidget(sn_debug *);

    void appendContextMenu(Menu *) override;
};
