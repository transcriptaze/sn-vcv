#include "plugin.hpp"
#include "sn.hpp"

#include "antialias/AA.hpp"
#include "antialias/FFT.hpp"
#include "dc-blocking/DCF.hpp"
#include "widgets/aliasing.hpp"
#include "widgets/xlight.hpp"

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

    void recompute(const ProcessArgs &args, DCBLOCK dcblocking);
    void processVCO(const ProcessArgs &args, size_t channels, ANTIALIAS antialias, DCBLOCK dcblocking, bool);
    void processAUX(const ProcessArgs &args, bool);
    void processFFT(const ProcessArgs &args, size_t channels);

    int krate();
    bool isLinkedLeft();
    bool isLinkedRight();

    // ... instance variables
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
    struct {
        AA out;
        AA sum;
    } AA;

    struct {
        FFTx fftx;
        FFT out;
        FFT sum;
    } fft;

    // ... DC  blocking
    struct {
        DCF out;
        DCF sum;
    } dcf = {
        .out = DCF(DCBLOCK_NONE),
        .sum = DCF(DCBLOCK_NONE),
    };
};

struct sn_vcoxWidget : ModuleWidget {
    sn_vcoxWidget(sn_vcox *);

    void appendContextMenu(Menu *) override;
};

struct sn_vcox_aliasing : AliasingWidget {
    sn_vcox_aliasing() {
        module = NULL;

        fontPath = asset::system("res/fonts/Nunito-Bold.ttf");
        text = "0.00";
    }

    void step() override {
        Widget::step();

        if (module != NULL) {
            char s[16];

            level = clamp(module->fft.out.q, 0.f, 1.f);
            enabled = module->fft.fftx.rate != FFT_RATE::OFF;
            mode = module->AA.out.mode;

            snprintf(s, sizeof(s), "%.0f%%", 5.0 * std::round(20.0 * level));
            text = s;
        }
    }

    sn_vcox *module;
};
