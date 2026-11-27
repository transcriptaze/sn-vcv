#include "plugin.hpp"

namespace sn_nlm {

struct sn_nlmx : Module {
    enum ParamId {
        PARAMS_LEN
    };

    enum InputId {
        ECCENTRICITY_INPUT,
        SENSITIVITY_INPUT,
        ROTATION_INPUT,
        AMPLITUDE_INPUT, // NTS: placeholder - not used
        DX_INPUT,
        DY_INPUT,
        PHI_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        OUTPUTS_LEN
    };

    enum LightId {
        LIGHTS_LEN
    };

    sn_nlmx();

    void process(const ProcessArgs &args) override;
};

struct sn_nlmxWidget : ModuleWidget {
    sn_nlmxWidget(sn_nlmx *);

    void appendContextMenu(Menu *) override;
};

} // namespace sn_nlm
