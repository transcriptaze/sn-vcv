#pragma once

#include "plugin.hpp"
#include "sn.hpp"

typedef struct range {
    int index;
    float defval;
    float base;
    float multiplier;
    float offset;
    int precision;

    float frequency(float v) {
        if (base > 0.f) {
            return multiplier * std::pow(base, v) + offset;
        } else {
            return multiplier * v + offset;
        }
    };
} range;

struct sn_lfo : Module {
    static const int CHANNELS;

    enum ParamId {
        ECCENTRICITY_PARAM,
        SENSITIVITY_PARAM,
        ROTATION_PARAM,
        AMPLITUDE_PARAM,
        DX_PARAM,
        DY_PARAM,
        PHI_PARAM,
        M_PARAM,
        FREQUENCY_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        FREQUENCY_INPUT,
        SYNCH_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        AUX_OUTPUT,
        AUX_TRIGGER,
        LFO_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        XLL_LIGHT,
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_lfo();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void onExpanderChange(const ExpanderChangeEvent &) override;
    void process(const ProcessArgs &) override;

    void recompute();
    void processLFO(const ProcessArgs &args, int, bool, bool, bool);
    void processAUX(const ProcessArgs &args, bool, bool);

    int channels();
    int getRange();
    void setRange(int);

    // ... instance variables
    dsp::PulseGenerator trigger;
    dsp::SchmittTrigger sync;

    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct LFO lfo[16] = {
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.lfo = 0.f, .sum = 0.f}},
    };

    struct AUX aux = {
        .mode = POLY,
        .gain = 0,
        .phase = 0.f,
        .out = {.osc = 0.f, .sum = 0.f},
    };

    // ... expanders
    struct {
        sn_expander<sn_lfo_message> left;
        sn_expander<sn_lfo_message> right;
    } expanders = {
        .left = sn_expander<sn_lfo_message>(LEFT),
        .right = sn_expander<sn_lfo_message>(RIGHT),
    };

    // ... state update
    struct {
        int krate;
        int count;
    } update{
        .krate = 0,
        .count = 0,
    };

    struct range range = {
        .index = 0,
        .defval = 5.f,
        .base = 1.5848933f,
        .multiplier = 0.1f,
        .offset = 0.f,
        .precision = 2,
    };
};

struct sn_lfoWidget : ModuleWidget {
    sn_lfoWidget(sn_lfo *);

    void appendContextMenu(Menu *) override;
};
