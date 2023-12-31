#include "plugin.hpp"
#include "sn.hpp"

struct sn_vco : Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum ParamId {
        ECCENTRICITY_PARAM,
        SENSITIVITY_PARAM,
        ROTATION_PARAM,
        AMPLITUDE_PARAM,
        DX_PARAM,
        DY_PARAM,
        PLACEHOLDER_PARAM,
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
        PITCH_INPUT,
        VELOCITY_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        AUX_OUTPUT,
        AUX_TRIGGER,
        VCO_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        XLL_LIGHT,
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_vco();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void onExpanderChange(const ExpanderChangeEvent &) override;
    void process(const ProcessArgs &args) override;

    void recompute();
    void processVCO(const ProcessArgs &args, int, bool);
    void processAUX(const ProcessArgs &args, bool);

    int channels();
    float velocity(int);

    // ... instance variables
    dsp::PulseGenerator trigger;

    struct AUX aux = {
        .mode = POLY,
        .gain = 0,
        .phase = 0.f,
        .out = {.osc = 0.f, .sum = 0.f},
    };

    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct VCO vco[16] = {
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
        {.phase = 0.f, .velocity = 0.f, .out = {.vco = 0.f, .sum = 0.f}},
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
        sn_expander<sn_vco_message> left;
        sn_expander<sn_vco_message> right;
    } expanders = {
        .left = sn_expander<sn_vco_message>(LEFT),
        .right = sn_expander<sn_vco_message>(RIGHT),
    };
};

struct sn_vcoWidget : ModuleWidget {
    sn_vcoWidget(sn_vco *);

    void appendContextMenu(Menu *) override;
};

struct sn_vco_channels : ChannelsWidget {
    sn_vco_channels() {
        fontPath = asset::system("res/fonts/DSEG7ClassicMini-BoldItalic.ttf");
        textPos = Vec(22, 20);
        bgText = "18";
        fontSize = 16;
    }

    void step() override {
        int channels = module ? module->channels() : 0;

        text = string::f("%d", channels);
    }

    sn_vco *module;
};
