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

constexpr struct {
    const double a[4];
    const double b[5];
} BUTTERWORTH_10kHz_4x{
    .a = {-3.146633, 3.785516, -2.054454, 0.423306},
    .b = {0.000483, 0.001934, 0.002901, 0.001934, 0.000483},
};

constexpr struct {
    const double a[4];
    const double b[5];
} BUTTERWORTH_18kHz_4x{
    .a = {-2.470272, 2.477971, -1.154635, 0.208848},
    .b = {0.003870, 0.015478, 0.023217, 0.015478, 0.003870},
};

constexpr struct {
    const double taps[100];
} FIR_10kHz_4x{
    // clang-format off
    .taps = {
       -2.42040533e-04, -8.63520763e-05,  9.11930380e-05,  2.84429089e-04,
        4.82833284e-04,  6.69590756e-04,  8.20956807e-04,  9.07480719e-04,
        8.97327522e-04,  7.61514313e-04,  4.80434600e-04,  5.06565066e-05,
       -5.09275348e-04, -1.15536763e-03, -1.81820490e-03, -2.40725216e-03,
       -2.81962722e-03, -2.95283356e-03, -2.72021883e-03, -2.06728701e-03,
       -9.86552627e-04,  4.71526304e-04,  2.19380873e-03,  4.00764923e-03,
        5.69285724e-03,  7.00220748e-03,  7.68908141e-03,  7.53968071e-03,
        6.40632933e-03,  4.23781240e-03,  1.10259796e-03, -2.79879473e-03,
       -7.13508758e-03, -1.14606405e-02, -1.52467354e-02, -1.79257577e-02,
       -1.89446092e-02, -1.78223232e-02, -1.42059792e-02, -7.91877410e-03,
        1.00544510e-03,  1.23057555e-02,  2.54987073e-02,  3.99046100e-02,
        5.46940328e-02,  6.89507948e-02,  8.17458427e-02,  9.22150994e-02,
        9.96337580e-02,  1.03479700e-01,  1.03479700e-01,  9.96337580e-02,
        9.22150994e-02,  8.17458427e-02,  6.89507948e-02,  5.46940328e-02,
        3.99046100e-02,  2.54987073e-02,  1.23057555e-02,  1.00544510e-03,
       -7.91877410e-03, -1.42059792e-02, -1.78223232e-02, -1.89446092e-02,
       -1.79257577e-02, -1.52467354e-02, -1.14606405e-02, -7.13508758e-03,
       -2.79879473e-03,  1.10259796e-03,  4.23781240e-03,  6.40632933e-03,
        7.53968071e-03,  7.68908141e-03,  7.00220748e-03,  5.69285724e-03,
        4.00764923e-03,  2.19380873e-03,  4.71526304e-04, -9.86552627e-04,
       -2.06728701e-03, -2.72021883e-03, -2.95283356e-03, -2.81962722e-03,
       -2.40725216e-03, -1.81820490e-03, -1.15536763e-03, -5.09275348e-04,
        5.06565066e-05,  4.80434600e-04,  7.61514313e-04,  8.97327522e-04,
        9.07480719e-04,  8.20956807e-04,  6.69590756e-04,  4.82833284e-04,
        2.84429089e-04,  9.11930380e-05, -8.63520763e-05, -2.42040533e-04,    },
    // clang-format on
};

constexpr struct {
    const double taps[4][25];
} POLYPHASE_18kHz_4x{
    // clang-format off
    .taps = {
        {
            -3.97413335e-04,  6.59195838e-04, -6.71203177e-04, -1.72181350e-04,
             2.18356030e-03, -4.32920111e-03,  4.10029020e-03,  9.19749721e-04,
            -1.04580445e-02,  1.97269540e-02, -1.95196199e-02, -5.51179954e-03,
             1.63591542e-01,  1.25898134e-01, -2.98487457e-02, -2.67726530e-03,
             1.38819673e-02, -1.22817019e-02,  5.43046798e-03,  5.75373833e-04,
            -3.03006506e-03,  2.50666309e-03, -9.83440823e-04, -9.14515086e-05,
             4.67012328e-04,
        },
        {
            -1.54078989e-04,  6.49144451e-04, -1.15016433e-03,  9.36230975e-04,
             9.17036077e-04, -4.24804639e-03,  6.80009830e-03, -4.84665351e-03,
            -4.30926472e-03,  1.93615905e-02, -3.34732299e-02,  3.27022539e-02,
             1.84640003e-01,  7.92496905e-02, -3.85123472e-02,  1.15115548e-02,
             4.73504725e-03, -9.93345919e-03,  7.46714654e-03, -2.50899043e-03,
            -1.02281745e-03,  1.97523848e-03, -1.30684360e-03,  3.89366620e-04,
             1.62716772e-04,
        },
        {
             1.62716772e-04,  3.89366620e-04, -1.30684360e-03,  1.97523848e-03,
            -1.02281745e-03, -2.50899043e-03,  7.46714654e-03, -9.93345919e-03,
             4.73504725e-03,  1.15115548e-02, -3.85123472e-02,  7.92496905e-02,
             1.84640003e-01,  3.27022539e-02, -3.34732299e-02,  1.93615905e-02,
            -4.30926472e-03, -4.84665351e-03,  6.80009830e-03, -4.24804639e-03,
             9.17036077e-04,  9.36230975e-04, -1.15016433e-03,  6.49144451e-04,
            -1.54078989e-04,
        },
        {
             4.67012328e-04, -9.14515086e-05, -9.83440823e-04,  2.50666309e-03,
            -3.03006506e-03,  5.75373833e-04,  5.43046798e-03, -1.22817019e-02,
             1.38819673e-02, -2.67726530e-03, -2.98487457e-02,  1.25898134e-01,
             1.63591542e-01, -5.51179954e-03, -1.95196199e-02,  1.97269540e-02,
            -1.04580445e-02,  9.19749721e-04,  4.10029020e-03, -4.32920111e-03,
             2.18356030e-03, -1.72181350e-04, -6.71203177e-04,  6.59195838e-04,
            -3.97413335e-04,
        },
    },
    // clang-format on
};

std::vector<std::pair<sn_debug::MODE, std::string>> MODES = {
    {sn_debug::BASELINE, "baseline"},
    {sn_debug::IIR, "IIR"},
    {sn_debug::FIR, "FIR"},
    {sn_debug::FIR4, "FIR4"},
};

sn_debug::sn_debug() {
    INFO("sn-debug v0.0.0::nl");

    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(IN1_INPUT, "1V/octave pitch");
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

    case IIR:
        process_IIR(args);
        break;

    case FIR:
        process_FIR(args);
        break;

    case FIR4:
        process_FIR4(args);
    }
}

void sn_debug::process_baseline(const ProcessArgs &args) {
    const float pitch = inputs[IN1_INPUT].getVoltage();
    const float f = dsp::FREQ_C4 * std::pow(2.f, pitch);
    const double δt = args.sampleTime;
    const double δφ = f * δt / 4;

    double in[4];

    for (int p = 0; p < 4; p++) {
        baseline.φ += δφ;

        const double α = 2.0 * M_PI * baseline.φ;
        in[p] = static_cast<float>(5.0 * std::sin(α));
    }

    float ref = in[3];
    float out = in[3];

    outputs[OUTA_OUTPUT].setChannels(2);
    outputs[OUTA_OUTPUT].setVoltage(ref, 0);
    outputs[OUTA_OUTPUT].setVoltage(out, 1);
}

void sn_debug::process_IIR(const ProcessArgs &args) {
    const float pitch = inputs[IN1_INPUT].getVoltage();
    const float f = dsp::FREQ_C4 * std::pow(2.f, pitch);
    const double δt = args.sampleTime;
    const double δφ = f * δt / 4;

    double in[4];

    for (int p = 0; p < 4; p++) {
        iir.φ += δφ;

        const double α = 2.0 * M_PI * iir.φ;
        in[p] = static_cast<float>(5.0 * std::sin(α));
    }

    float ref = in[3];
    float out = butterworth(iir.iir, in);

    outputs[OUTA_OUTPUT].setChannels(2);
    outputs[OUTA_OUTPUT].setVoltage(ref, 0);
    outputs[OUTA_OUTPUT].setVoltage(out, 1);
}

void sn_debug::process_FIR(const ProcessArgs &args) {
    const float pitch = inputs[IN1_INPUT].getVoltage();
    const float f = dsp::FREQ_C4 * std::pow(2.f, pitch);
    const double δt = args.sampleTime;
    const double δφ = f * δt / 4;

    double in[4];

    for (int p = 0; p < 4; p++) {
        fir.φ += δφ;

        const double α = 2.0 * M_PI * fir.φ;
        in[p] = static_cast<float>(5.0 * std::sin(α));
    }

    float ref = in[3];
    float out = 0.0f;

    for (int p = 0; p < 4; p++) {
        fir.delay_line[fir.head] = in[p];

        out = 0.0f;

        for (int i = 0; i < 100; ++i) {
            int ix = (fir.head + 100 - i) % 100;
            out += FIR_10kHz_4x.taps[i] * fir.delay_line[ix];
        }

        fir.head = (fir.head + 1) % 100;
    }

    outputs[OUTA_OUTPUT].setChannels(2);
    outputs[OUTA_OUTPUT].setVoltage(ref, 0);
    outputs[OUTA_OUTPUT].setVoltage(out, 1);
}

void sn_debug::process_FIR4(const ProcessArgs &args) {
    const float pitch = inputs[IN1_INPUT].getVoltage();
    const float f = dsp::FREQ_C4 * std::pow(2.f, pitch);
    const double δt = args.sampleTime;
    const double δφ = f * δt / 4;

    // ... 4x oversampled sine wave
    double in[4];

    for (int p = 0; p < 4; p++) {
        fir4.φ += δφ;

        const double α = 2.0 * M_PI * fir4.φ;
        in[p] = static_cast<float>(5.0 * std::sin(α));
    }

    // ... polyphase decimator
    int ix = 1;
    for (int p = 0; p < 4; ++p) {
        for (int i = 24; i > 0; --i) {
            fir4.state[ix] = fir4.state[ix - 1];
            ix++;
        }
    }

    fir4.state[0] = in[3];
    fir4.state[25] = in[2];
    fir4.state[50] = in[1];
    fir4.state[75] = in[0];

    float ref = in[3];
    float out = 0.0f;

    int jx = 0;
    for (size_t p = 0; p < 4; ++p) {
        float acc = 0.0f;

        for (size_t i = 0; i < 25; ++i) {
            acc += POLYPHASE_18kHz_4x.taps[p][i] * fir4.state[jx];

            jx++;
        }

        out += acc;
    }

    outputs[OUTA_OUTPUT].setChannels(2);
    outputs[OUTA_OUTPUT].setVoltage(ref, 0);
    outputs[OUTA_OUTPUT].setVoltage(out, 1);
}

float sn_debug::butterworth(struct iir4 &iir, double in[4]) {
    for (size_t i = 0; i < 4; i++) {
        double out = 0.0f;

        out = BUTTERWORTH_10kHz_4x.b[0] * in[i];
        out += BUTTERWORTH_10kHz_4x.b[1] * iir.x[0];
        out += BUTTERWORTH_10kHz_4x.b[2] * iir.x[1];
        out += BUTTERWORTH_10kHz_4x.b[3] * iir.x[2];
        out += BUTTERWORTH_10kHz_4x.b[4] * iir.x[3];

        out -= BUTTERWORTH_10kHz_4x.a[0] * iir.y[0];
        out -= BUTTERWORTH_10kHz_4x.a[1] * iir.y[1];
        out -= BUTTERWORTH_10kHz_4x.a[2] * iir.y[2];
        out -= BUTTERWORTH_10kHz_4x.a[3] * iir.y[3];

        iir.x[3] = iir.x[2];
        iir.x[2] = iir.x[1];
        iir.x[1] = iir.x[0];
        iir.x[0] = in[i];

        iir.y[3] = iir.y[2];
        iir.y[2] = iir.y[1];
        iir.y[1] = iir.y[0];
        iir.y[0] = out;
    }

    return iir.y[0];
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
