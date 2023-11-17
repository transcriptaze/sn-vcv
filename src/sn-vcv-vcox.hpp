#include "plugin.hpp"
#include "sn-vcv.hpp"

struct sn_vcv_vcox : Module {
    static const int CHANNELS;

    enum ParamId {
        ECCENTRICITY_PARAM,
        SENSITIVITY_PARAM,
        ROTATION_PARAM,
        AMPLITUDE_PARAM,
        DX_PARAM,
        DY_PARAM,
        PSI_PARAM,
        M_PARAM,
        PARAMS_LEN
    };

    enum InputId {
        ECCENTRICITY_INPUT,
        SENSITIVITY_INPUT,
        ROTATION_INPUT,
        AMPLITUDE_INPUT,
        DX_INPUT,
        DY_INPUT,
        PSI_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        VCO_OUTPUT,
        VCO_SUM_OUTPUT,
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

    sn_vcv_vcox();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void onExpanderChange(const ExpanderChangeEvent &) override;
    void process(const ProcessArgs &args) override;

    void recompute();
    void processVCO(const ProcessArgs &args, bool);
    void processAUX(const ProcessArgs &args, bool);

    int krate();
    bool isLinkedLeft();
    bool isLinkedRight();

    // ... instance variables
    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct Ζ ζ = {
        .pʼ = 1.f,
        .qʼ = 0.f,
        .rʼ = 0.f,
        .sʼ = 1.f,
        .tʼ = 0.f,
        .uʼ = 0.f,
        .φ = 0.f,
    };

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
};

struct sn_vcv_vcoxWidget : ModuleWidget {
    sn_vcv_vcoxWidget(sn_vcv_vcox *);

    void appendContextMenu(Menu *);
};