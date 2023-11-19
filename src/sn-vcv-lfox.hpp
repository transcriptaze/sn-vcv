#pragma once

#include "plugin.hpp"
#include "sn-vcv.hpp"

struct sn_vcv_lfox : Module {
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
        PARAMS_LEN
    };

    enum InputId {
        INPUTS_LEN
    };

    enum OutputId {
        LFO_OUTPUT,
        SUM_OUTPUT,
        AUX_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        XLL_LIGHT,
        XLR_LIGHT,
        XRL_LIGHT,
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_vcv_lfox();

    json_t *dataToJson() override;
    void dataFromJson(json_t *root) override;
    void onExpanderChange(const ExpanderChangeEvent &) override;
    void process(const ProcessArgs &) override;

    bool isLinkedLeft();
    bool isLinkedRight();

    void recompute();
    void processAUX(const ProcessArgs &, bool);

    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct {
        float phase;
        struct {
            float osc;
            float sum;
        } out;
    } LFO[16] = {
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
        {.phase = 0.f, .out = {.osc = 0.f, .sum = 0.f}},
    };

    struct {
        enum AUXMODE mode;
        float phase;
        struct {
            float osc;
            float sum;
        } out;
    } aux = {
        .mode = POLY,
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

struct sn_vcv_lfoxWidget : ModuleWidget {
    sn_vcv_lfoxWidget(sn_vcv_lfox *m);

    void appendContextMenu(Menu *) override;
};