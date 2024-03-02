#include "plugin.hpp"
#include "sn.hpp"

#include "antialias/AAF.hpp"

struct sn_vcox : Module {
    static const int CHANNELS;

    enum ParamId {
        ECCENTRICITY_PARAM,
        SENSITIVITY_PARAM,
        ROTATION_PARAM,
        AMPLITUDE_PARAM,
        DX_PARAM,
        DY_PARAM,
        PLACEHOLDER_PARAM,
        M_PARAM,
        ATT_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        ECCENTRICITY_INPUT,
        SENSITIVITY_INPUT,
        ROTATION_INPUT,
        AMPLITUDE_INPUT,
        DX_INPUT,
        DY_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        AUX_OUTPUT,
        VCO_OUTPUT,
        VCO_SUM_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        XLL_LIGHT,
        XLR_LIGHT,
        XRL_LIGHT,
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_vcox();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void onExpanderChange(const ExpanderChangeEvent &) override;
    void process(const ProcessArgs &args) override;

    void recompute(const ProcessArgs &args);
    void processVCO(const ProcessArgs &args, size_t, ANTIALIAS, bool);
    void processAUX(const ProcessArgs &args, bool);

    int krate();
    bool isLinkedLeft();
    bool isLinkedRight();

    // ... instance variables
    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct VCO vco[16] = {
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = .0f, .sum = 0.f}},
    };

    struct AUX aux = {
        .mode = POLY,
        .gain = 0,
        .phase = 0.f,
        .out = {.osc = 0.f, .sum = 0.f},
    };

    // ... state update
    struct {
        int krate;
        int count;
    } update{
        .krate = 0,
        .count = 0,
    };

    // ... expanders
    struct {
        bool linkedLeft;
        bool linkedRight;
        sn_expander<sn_vco_message> left;
        sn_expander<sn_vco_message> right;
    } expanders = {
        .linkedLeft = false,
        .linkedRight = false,
        .left = sn_expander<sn_vco_message>(LEFT),
        .right = sn_expander<sn_vco_message>(RIGHT),
    };

    // ... antialiasing
    struct AA AA = {
        .fs = 44100.f,
        .mode = NONE,
        .x1f1 = AAF(X1F1, 44100.f),
        .x1f2 = {AAF(X1F2, 44100.f), AAF(X1F2, 44100.f)},
        .x2f1 = AAF(X2F1, 44100.f),
    };
};

struct sn_vcoxWidget : ModuleWidget {
    sn_vcoxWidget(sn_vcox *);

    void appendContextMenu(Menu *) override;
};
