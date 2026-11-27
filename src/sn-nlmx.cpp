#include "sn-nlmx.hpp"

namespace sn_nlm {

const auto SN_NLMX_INPUT_E = mm2px(Vec(7.62, 36.25));
const auto SN_NLMX_INPUT_S = mm2px(Vec(7.62, 49.27));
const auto SN_NLMX_INPUT_THETA = mm2px(Vec(7.62, 62.28));
const auto SN_NLMX_INPUT_DX = mm2px(Vec(7.62, 75.29));
const auto SN_NLMX_INPUT_DY = mm2px(Vec(7.62, 88.31));
const auto SN_NLMX_INPUT_PHI = mm2px(Vec(7.62, 101.32));

sn_nlmx::sn_nlmx() {
    INFO("sn-nlmx v0.0.0");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // ... inputs
    // configInput(IN_INPUT, "±10V in");
}

void sn_nlmx::process(const ProcessArgs &args) {
}

sn_nlmxWidget::sn_nlmxWidget(sn_nlmx *module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-nlmx.svg"),
                         asset::plugin(pluginInstance, "res/sn-nlmx-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... inputs
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLMX_INPUT_E, module, sn_nlmx::ECCENTRICITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLMX_INPUT_S, module, sn_nlmx::SENSITIVITY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLMX_INPUT_THETA, module, sn_nlmx::ROTATION_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLMX_INPUT_DX, module, sn_nlmx::DX_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLMX_INPUT_DY, module, sn_nlmx::DY_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(SN_NLMX_INPUT_PHI, module, sn_nlmx::PHI_INPUT));
}

void sn_nlmxWidget::appendContextMenu(Menu *menu) {
    // sn_nlmx *module = getModule<sn_nlmx>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-nlmx settings"));
}

} // namespace sn_nlm

Model *model_sn_nlmx = createModel<sn_nlm::sn_nlmx, sn_nlm::sn_nlmxWidget>("sn-nlmx");
