#include "sn-nlm.hpp"

sn_nlm::sn_nlm() {
    INFO("sn-nlm v0.0.0");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

    // ... params
    configParam<LogGain>(GAIN_PARAM, -1.0f, 1.0f, 0.0f, "gain", "");

    getParamQuantity(GAIN_PARAM)->randomizeEnabled = false;

    // ... inputs
    configInput(IN1_INPUT, "±5V in1");
    configInput(IN2_INPUT, "±5V in2");
    configInput(DEBUG_INPUT, "debug");

    // ... outputs
    configOutput(OUTA_OUTPUT, "outA");
}

json_t *sn_nlm::dataToJson() {
    json_t *root = json_object();

    return root;
}

void sn_nlm::dataFromJson(json_t *root) {
}

void sn_nlm::process(const ProcessArgs &args) {
    float in1 = inputs[IN1_INPUT].getPolyVoltage(0);
    float in2 = inputs[IN2_INPUT].getPolyVoltage(0);
    float gain = params[GAIN_PARAM].getValue();

    this->gain = std::powf(2.0f, 2.0f * gain);

    float outA = hard_limit(in1, in2, this->gain);

    outputs[OUTA_OUTPUT].setVoltage(outA, 0);
}

float sn_nlm::hard_limit(float in1, float in2, float gain) {
    return clamp(in1 + in2, -5.f, 5.f);
}

sn_nlmWidget::sn_nlmWidget(sn_nlm *module) {
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
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-nlm.svg"),
                         asset::plugin(pluginInstance, "res/sn-nlm-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // ... inputs
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input1), module, sn_nlm::IN1_INPUT));
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(input2), module, sn_nlm::IN2_INPUT));

    // ... outputs
    addOutput(createOutputCentered<ThemedPJ301MPort>(mm2px(outA), module, sn_nlm::OUTA_OUTPUT));

    // ... parameters
    addParam(createParamCentered<RoundBlackKnob>(SN_NLM_PARAM_GAIN, module, sn_nlm::GAIN_PARAM));

    // ... debug
    addInput(createInputCentered<ThemedPJ301MPort>(mm2px(debug), module, sn_nlm::DEBUG_INPUT));
}

void sn_nlmWidget::appendContextMenu(Menu *menu) {
    // sn_nlm *module = getModule<sn_nlm>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-nlm settings"));
}

Model *model_sn_nlm = createModel<sn_nlm, sn_nlmWidget>("sn-nlm");
