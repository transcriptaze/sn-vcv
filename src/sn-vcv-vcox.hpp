#include "plugin.hpp"
#include "sn-vcv.hpp"

struct sn_vcv_vcox : Module {
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
        XRR_LIGHT,
        LIGHTS_LEN
    };

    sn_vcv_vcox();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;

    void process(const ProcessArgs &args) override;
    void processVCO(const ProcessArgs &args, bool);
    void processAUX(const ProcessArgs &args, bool);
    void recompute();

    int krate();

    // ... instance variables
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

    float phase[16] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
    float out[16] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

    // ... state update
    struct {
        int krate;
        int count;
    } update{
        .krate = 0,
        .count = 0,
    };
};

struct sn_vcv_vcoxWidget : ModuleWidget {
    sn_vcv_vcoxWidget(sn_vcv_vcox *);

    void appendContextMenu(Menu *);
};
