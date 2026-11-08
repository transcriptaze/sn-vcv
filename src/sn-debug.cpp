#include "sn-debug.hpp"

using rack::simd::float_4;

const int sn_debug::CHANNELS = 1;
const float sn_debug::VELOCITY = 1.0f;

const auto SN_DEBUG_INPUT_PITCH = mm2px(Vec(7.33, 36.25));
const auto SN_DEBUG_OUTPUT_VCO = mm2px(Vec(7.33, 114.34));

const std::vector<std::string> MODES = {
    "baseline",
    "baseline (per channel)",
    "baseline (per channel, std::array)",
    "sine",
    "sn::υ",
    "sn::υ4",
    "sn::υ4 (double)",
    "sn::υ4d",
};

// baseline
constexpr double TWO_TO_32 = 4294967296.0; // 2^32
constexpr uint32_t MAX_RAMP = 2147483647U;

sn_debug::sn_debug() {
    INFO("sn-debug v0.0.0::simd");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(PITCH_INPUT, "1V/octave pitch");
    configOutput(VCO_OUTPUT, "VCO");

    sn_υ.sn.recompute();
    sn_υ4.sn.recompute();
    sn_υ4_double.sn.recompute();
    sn_υ4d.sn.recompute();
}

json_t *sn_debug::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "mode", json_integer(mode));

    return root;
}

void sn_debug::dataFromJson(json_t *root) {
    json_t *m = json_object_get(root, "mode");

    if (m) {
        switch (json_integer_value(m)) {
        case BASELINE:
            mode = BASELINE;
            break;

        case BASELINE_PER_CHANNEL:
            mode = BASELINE_PER_CHANNEL;
            break;

        case BASELINE_STD_ARRAY:
            mode = BASELINE_STD_ARRAY;
            break;

        case SINE:
            mode = SINE;
            break;

        case SN_V:
            mode = SN_V;
            break;

        case SN_V4:
            mode = SN_V4;
            break;

        case SN_V4_DOUBLE:
            mode = SN_V4_DOUBLE;
            break;

        case SN_V4D:
            mode = SN_V4D;
            break;
        }
    }
}

void sn_debug::process(const ProcessArgs &args) {
    switch (mode) {
    case BASELINE:
        process_baseline(args);
        break;

    case BASELINE_PER_CHANNEL:
        process_baseline_per_channel(args);
        break;

    case BASELINE_STD_ARRAY:
        process_baseline_std_array(args);
        break;

    case SINE:
        process_sine(args);
        break;

    case SN_V:
        process_sn_υ(args);
        break;

    case SN_V4:
        process_sn_υ4(args);
        break;

    case SN_V4_DOUBLE:
        process_sn_υ4_double(args);
        break;

    case SN_V4D:
        process_sn_υ4d(args);
        break;
    }
}

void sn_debug::process_baseline(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    uint32_t δα = static_cast<uint32_t>((20.0 / args.sampleRate) * TWO_TO_32);

    baseline.α += δα;
    bool msb = (baseline.α >> 31) & 1;
    uint32_t ramp = baseline.α & 0x7FFFFFFF;
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

void sn_debug::process_baseline_per_channel(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    uint32_t δα = static_cast<uint32_t>((20.0 / args.sampleRate) * TWO_TO_32);

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        baseline_per_channel.α[ch] += δα;

        bool msb = (baseline_per_channel.α[ch] >> 31) & 1;
        uint32_t ramp = baseline_per_channel.α[ch] & 0x7FFFFFFF;
        if (msb) {
            ramp = MAX_RAMP - ramp;
        }

        double normalized = (static_cast<double>(ramp) / static_cast<double>(MAX_RAMP)) * 2.0 - 1.0;
        float out = normalized * 5.0f;

        outputs[VCO_OUTPUT].setVoltage(out, ch);
    }
}

void sn_debug::process_baseline_std_array(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;
    uint32_t δα = static_cast<uint32_t>((20.0 / args.sampleRate) * TWO_TO_32);

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        baseline_std_array.α[ch] += δα;

        bool msb = (baseline_std_array.α[ch] >> 31) & 1;
        uint32_t ramp = baseline_std_array.α[ch] & 0x7FFFFFFF;
        if (msb) {
            ramp = MAX_RAMP - ramp;
        }

        double normalized = (static_cast<double>(ramp) / static_cast<double>(MAX_RAMP)) * 2.0 - 1.0;
        float out = normalized * 5.0f;

        outputs[VCO_OUTPUT].setVoltage(out, ch);
    }
}

void sn_debug::process_sine(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    const double f = 200.0;
    const double δt = args.sampleTime;
    const double δφ = f * δt;

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        sine.φ[ch] += δφ;

        const double α = sine.φ[ch];
        const float out = 5.0f * std::sin(α);

        outputs[VCO_OUTPUT].setVoltage(out, ch);
    }
}

void sn_debug::process_sn_υ(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    const double f = 200.0;
    const double δt = args.sampleTime;
    const double δφ = f * δt;

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        sn_υ.φ[ch] += δφ;

        const double α = sn_υ.φ[ch];
        const float out = 5.0f * sn_υ.sn.υ(α);

        outputs[VCO_OUTPUT].setVoltage(out, ch);
    }
}

void sn_debug::process_sn_υ4(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    const float f = 200.0;
    const float δt = args.sampleTime;
    const float δφ = f * δt;

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        sn_υ4.φ[ch] += δφ;

        const float α = sn_υ4.φ[ch];
        const float_4 out = 5.0f * sn_υ4.sn.υ4(α, δφ);

        outputs[VCO_OUTPUT].setVoltage(out[0], ch);
    }
}

void sn_debug::process_sn_υ4_double(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    const double f = 200.0;
    const double δt = args.sampleTime;
    const double δφ = f * δt;

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        sn_υ4.φ[ch] += δφ;

        const double α = sn_υ4.φ[ch];
        const float_4 out = 5.0f * sn_υ4_double.sn.υ4(α, δφ);

        outputs[VCO_OUTPUT].setVoltage(out[0], ch);
    }
}

void sn_debug::process_sn_υ4d(const ProcessArgs &args) {
    int channels = inputs[PITCH_INPUT].isConnected() ? inputs[PITCH_INPUT].getChannels() : CHANNELS;

    const double f = 200.0;
    const double δt = args.sampleTime;
    const double δφ = f * δt;

    outputs[VCO_OUTPUT].setChannels(channels);

    for (int ch = 0; ch < channels; ch++) {
        inputs[PITCH_INPUT].getPolyVoltage(ch);

        sn_υ4d.φ[ch] += δφ;

        const double α = sn_υ4d.φ[ch];
        const float_4 out = 5.0f * sn_υ4d.sn.υ4(α, δφ);

        outputs[VCO_OUTPUT].setVoltage(out[0], ch);
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
    sn_debug *module = getModule<sn_debug>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-debug settings"));

    menu->addChild(createIndexPtrSubmenuItem("mode",
                                             MODES,
                                             &module->mode));
}

Model *model_sn_debug = createModel<sn_debug, sn_debugWidget>("sn-debug");
