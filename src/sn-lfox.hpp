#pragma once

#include "plugin.hpp"
#include "sn.hpp"

struct sn_lfox : Module {
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
        ATT_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        INPUTS_LEN
    };

    enum OutputId {
        AUX_OUTPUT,
        LFO_OUTPUT,
        SUM_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        XLL_LIGHT,
        XLR_LIGHT,
        XRL_LIGHT,
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_lfox();

    json_t *dataToJson() override;
    void dataFromJson(json_t *root) override;
    void onExpanderChange(const ExpanderChangeEvent &) override;
    void process(const ProcessArgs &) override;

    bool isLinkedLeft();
    bool isLinkedRight();

    void recompute();
    void processLFO(const ProcessArgs &, int, bool, bool);
    void processAUX(const ProcessArgs &, bool);

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

    struct {
        bool linkLeft;
        bool linkRight;
        sn_expander<sn_lfo_message> left;
        sn_expander<sn_lfo_message> right;
    } expanders = {
        .linkLeft = false,
        .linkRight = false,
        .left = sn_expander<sn_lfo_message>(LEFT),
        .right = sn_expander<sn_lfo_message>(RIGHT),
    };

    struct {
        int krate;
        int count;
    } update{
        .krate = 0,
        .count = 0,
    };
};

struct sn_lfoxWidget : ModuleWidget {
    sn_lfoxWidget(sn_lfox *m);

    void appendContextMenu(Menu *) override;
};
