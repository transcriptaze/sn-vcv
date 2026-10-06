#include "sn-sigma.hpp"

sn_sigma::sn_sigma() {
    // ... params
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // ... inputs
    configInput(IN1_INPUT, "±5V in1");
    configInput(IN2_INPUT, "±5V in2");
    configInput(DEBUG_INPUT, "debug");

    // ... outputs
    configOutput(OUTA_OUTPUT, "outA");
}

json_t *sn_sigma::dataToJson() {
    json_t *root = json_object();

    return root;
}

void sn_sigma::dataFromJson(json_t *root) {
}

void sn_sigma::process(const ProcessArgs &args) {
    float in1 = inputs[IN1_INPUT].getPolyVoltage(0);
    float in2 = inputs[IN2_INPUT].getPolyVoltage(0);
    float outA = hard_max(in1, in2);

    outputs[OUTA_OUTPUT].setVoltage(outA, 0);
}

float sn_sigma::hard_max(float in1, float in2) {
    return clamp(in1 + in2, -5.f, 5.f);
}

sn_sigmaWidget::sn_sigmaWidget(sn_sigma *module) {
    float left = 7.331;
    // float middle = 20.351;
    float right = 35.56;
    float top = 21.968 + 1.27;
    float dh = 13.014;

    Vec input1(left, top);
    Vec input2(left, top + dh);
    Vec outA(right, top + 7 * dh + 1.27);
    Vec debug(left, top + 7 * dh + 1.27);

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-sigma.svg"),
                         asset::plugin(pluginInstance, "res/sn-sigma-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... inputs
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input1), module, sn_sigma::IN1_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input2), module, sn_sigma::IN2_INPUT));

    // ... outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(outA), module, sn_sigma::OUTA_OUTPUT));

    // ... debug
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(debug), module, sn_sigma::DEBUG_INPUT));
}

void sn_sigmaWidget::appendContextMenu(Menu *menu) {
    // sn_sigma *module = getModule<sn_sigma>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-sigma settings"));
}

Model *model_sn_sigma = createModel<sn_sigma, sn_sigmaWidget>("sn-sigma");
