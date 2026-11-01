#include "plugin.hpp"
#include "sn.hpp"

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

    void processVCO(const ProcessArgs &args, int channels);

    double φ[16] = {};
    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
};

struct sn_debugWidget : ModuleWidget {
    sn_debugWidget(sn_debug *);

    void appendContextMenu(Menu *) override;
};
