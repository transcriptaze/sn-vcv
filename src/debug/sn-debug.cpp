#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "sn-debug.hpp"

using namespace rack;

const int sn_debug::CHANNELS = 1;
const float sn_debug::VELOCITY = 1.0f;

const auto SN_DEBUG_INPUT_IN1 = mm2px(Vec(7.33, 36.25));
const auto SN_DEBUG_OUTPUT_A = mm2px(Vec(7.33, 114.34));

std::vector<std::pair<sn_debug::MODE, std::string>> MODES = {
    {sn_debug::BASELINE, "baseline"},
    {sn_debug::FIR4, "FIR4"},
};

sn_debug::sn_debug() {
    INFO("sn-debug v0.0.0::nl");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(IN1_INPUT, "±5V in");
    configOutput(OUTA_OUTPUT, "±5V out");
}

json_t *sn_debug::dataToJson() {
    json_t *root = json_object();

    for (auto ix = MODES.begin(); ix != MODES.end(); ix++) {
        if (mode == ix->first) {
            json_object_set_new(root, "mode", json_string(ix->second.c_str()));
            break;
        }
    }

    return root;
}

void sn_debug::dataFromJson(json_t *root) {
    json_t *m = json_object_get(root, "mode");

    if (m) {
        std::string s(json_string_value(m));

        INFO(">>>> sn-debug::mode %s", s.c_str());

        for (auto ix = MODES.begin(); ix != MODES.end(); ix++) {
            if (s == ix->second) {
                mode = ix->first;
                break;
            }
        }
    }
}

void sn_debug::process(const ProcessArgs &args) {
    switch (mode) {
    case BASELINE:
        process_baseline(args);
        break;

    case FIR4:
        process_FIR4(args);
    }
}

void sn_debug::process_baseline(const ProcessArgs &args) {
    int channels = inputs[IN1_INPUT].isConnected() ? inputs[IN1_INPUT].getChannels() : CHANNELS;

    outputs[OUTA_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        outputs[OUTA_OUTPUT].setVoltage(0.0f, ch);
    }
}

void sn_debug::process_FIR4(const ProcessArgs &args) {
    int channels = inputs[IN1_INPUT].isConnected() ? inputs[IN1_INPUT].getChannels() : CHANNELS;

    outputs[OUTA_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        outputs[OUTA_OUTPUT].setVoltage(0.0f, ch);
    }
}

sn_debugWidget::sn_debugWidget(sn_debug *module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-debug.svg"),
                         asset::plugin(pluginInstance, "res/sn-debug-dark.svg")));

    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ThemedScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ThemedScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addInput(createInputCentered<ThemedPJ301MPort>(SN_DEBUG_INPUT_IN1, module, sn_debug::IN1_INPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_DEBUG_OUTPUT_A, module, sn_debug::OUTA_OUTPUT));
}

void sn_debugWidget::appendContextMenu(Menu *menu) {
    sn_debug *module = getModule<sn_debug>();
    std::vector<std::string> modes;

    for (auto ix = MODES.begin(); ix != MODES.end(); ix++) {
        modes.push_back(ix->second);
    }

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-debug settings"));

    menu->addChild(createIndexPtrSubmenuItem("mode",
                                             modes,
                                             &module->mode));
}

Model *model_sn_debug = createModel<sn_debug, sn_debugWidget>("sn-debug");
