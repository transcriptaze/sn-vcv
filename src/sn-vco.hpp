#include "plugin.hpp"
#include "sn.hpp"

#include "antialias/AA.hpp"
#include "dc-blocking/DCF.hpp"
#include "widgets/xlight.hpp"

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
        DEBUG_INPUT,
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

    void recompute(const ProcessArgs &args, size_t);
    void processVCO(const ProcessArgs &args, size_t, bool);
    void processAUX(const ProcessArgs &args, bool);

    int channels();

    // ... instance variables
    dsp::PulseGenerator trigger;
    float frequency[16] = {dsp::FREQ_C4};
    float velocity[16] = {0.f};

    struct AUX aux = {
        .mode = POLY,
        .gain = 0,
        .phase = 0.f,
        .out = {.osc = 0.f, .sum = 0.f},
    };

    struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    struct VCO vco[16] = {
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
        {.α = 0.f, .velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {.vco = {0.f, 0.f, 0.f, 0.f}, .sum = {0.f, 0.f, 0.f, 0.f}}},
    };

    // ... anti-aliasing
    ANTIALIAS antialias = NONE; // ... for context submenu
    struct AA aa;

    // ... DC  blocking
    DCBLOCK dcblocking = DCBLOCK_NONE; // ... for context submenu
    struct DCF dcf = DCF(DCBLOCK_NONE);

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

    // ... debug
    dsp::SchmittTrigger debug;
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
        module = NULL;
    }

    void step() override {
        Widget::step();

        int channels = module ? module->channels() : 0;

        text = string::f("%d", channels);
    }

    sn_vco *module;
};
