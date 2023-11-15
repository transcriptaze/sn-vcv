#include "plugin.hpp"
#include "sn-vcv.hpp"

struct sn_vcv_vco : Module {
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
        PITCH_INPUT,
        VELOCITY_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        VCO_OUTPUT,
        AUX_OUTPUT,
        AUX_TRIGGER,
        OUTPUTS_LEN
    };

    enum LightId {
        XLL_LIGHT,
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_vcv_vco();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;

    void process(const ProcessArgs &args) override;
    void processAUX(const ProcessArgs &args, bool);
    void recompute();

    int channels();
    int krate();

    // ... instance variables
    dsp::PulseGenerator trigger;

    struct AUX aux = {
        .mode = POLY,
        .phase = 0.f,
        .out = {.osc = 0.f, .sum = 0.f},
    };

    struct SN sn {
        0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f
    };

    struct Ζ ζ = {
        .pʼ = 1.f,
        .qʼ = 0.f,
        .rʼ = 0.f,
        .sʼ = 1.f,
        .tʼ = 0.f,
        .uʼ = 0.f,
        .φ = 0.f,
    };

    // ... update state
    struct {
        int krate;
        int count;
    } update{
        .krate = 0,
        .count = 0,
    };
};

struct sn_vcv_vcoWidget : ModuleWidget {
    sn_vcv_vcoWidget(sn_vcv_vco *);

    void appendContextMenu(Menu *);
};

struct sn_vcv_vco_channels : ChannelsWidget {
    sn_vcv_vco_channels() {
        fontPath = asset::system("res/fonts/DSEG7ClassicMini-BoldItalic.ttf");
        textPos = Vec(22, 20);
        bgText = "18";
        fontSize = 16;
    }

    void step() override {
        int channels = module ? module->channels() : 0;

        text = string::f("%d", channels);
    }

    sn_vcv_vco *module;
};
