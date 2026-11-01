#include "sn-debug.hpp"

const int sn_debug::CHANNELS = 1;
const float sn_debug::VELOCITY = 1.0f;

const auto SN_DEBUG_INPUT_PITCH = mm2px(Vec(7.33, 36.25));
const auto SN_DEBUG_OUTPUT_VCO = mm2px(Vec(7.33, 114.34));

constexpr double TWO_TO_32 = 4294967296.0; // 2^32
constexpr uint32_t MAX_RAMP = 2147483647U;

sn_debug::sn_debug() {
    INFO("sn-debug v0.0.0::simd");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(PITCH_INPUT, "1V/octave pitch");
    configOutput(VCO_OUTPUT, "VCO");
}

void sn_debug::process(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    uint32_t δα = static_cast<uint32_t>((20.0 / args.sampleRate) * TWO_TO_32);

    α += δα;
    bool msb = (α >> 31) & 1;
    uint32_t ramp = α & 0x7FFFFFFF;
    if (msb) {
        ramp = MAX_RAMP - ramp;
    }

    double normalized = (static_cast<double>(ramp) / static_cast<double>(MAX_RAMP)) * 2.0 - 1.0;
    float out = normalized * 5.0f;

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);
    }

    outputs[VCO_OUTPUT].setChannels(16);

    for (int ch = 0; ch < 16; ch++) {
        outputs[VCO_OUTPUT].setVoltage(out, ch);
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

    addInput(createInputCentered<ThemedPJ301MPort>(SN_DEBUG_INPUT_PITCH, module, sn_debug::PITCH_INPUT));
    addOutput(createOutputCentered<ThemedPJ301MPort>(SN_DEBUG_OUTPUT_VCO, module, sn_debug::VCO_OUTPUT));
}

void sn_debugWidget::appendContextMenu(Menu *menu) {
    // sn_debug *module = getModule<sn_debug>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-debug settings"));
}

Model *model_sn_debug = createModel<sn_debug, sn_debugWidget>("sn-debug");
