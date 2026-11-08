#include "plugin.hpp"
#include "sn.hpp"
#include "sn_double.hpp"

struct sn_debug : Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum MODE {
        BASELINE,
        BASELINE_PER_CHANNEL,
        BASELINE_STD_ARRAY,
        SINE,
        SINE4,
        SINE_FAST,
        SINE_LOOKUP,
        SN_V,
        SN_V4,
        SN_V4_DOUBLE,
        SN_V4D,
    };

    enum ParamId {
        PARAMS_LEN
    };

    enum InputId {
        PITCH_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        VCO_OUTPUT,
        OUTPUTS_LEN
    };

    enum LightId {
        LIGHTS_LEN
    };

    sn_debug();

    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;
    void process(const ProcessArgs &args) override;

    void process_baseline(const ProcessArgs &args);
    void process_baseline_per_channel(const ProcessArgs &args);
    void process_baseline_std_array(const ProcessArgs &args);
    void process_sine(const ProcessArgs &args);
    void process_sine4(const ProcessArgs &args);
    void process_sine_fast(const ProcessArgs &args);
    void process_sine_lookup(const ProcessArgs &args);
    void process_sn_υ(const ProcessArgs &args);
    void process_sn_υ4(const ProcessArgs &args);
    void process_sn_υ4_double(const ProcessArgs &args);
    void process_sn_υ4d(const ProcessArgs &args);

    MODE mode = BASELINE;
    double φ[16] = {};
    struct SN sn = SN(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

    struct {
        uint32_t α = 0;
    } baseline;

    struct {
        uint32_t α[16] = {};
    } baseline_per_channel;

    struct {
        std::array<uint32_t, 16> α = {};
    } baseline_std_array;

    struct {
        double φ[16] = {};
    } sine;

    struct {
        float φ[16] = {};
    } sine4;

    struct {
        float φ[16] = {};
    } sine_fast;

    struct {
        double φ[16] = {};
    } sine_lookup;

    struct {
        double φ[16] = {};
        struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
    } sn_υ;

    struct {
        float φ[16] = {};
        struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
    } sn_υ4;

    struct {
        double φ[16] = {};
        struct SN sn = SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
    } sn_υ4_double;

    struct {
        double φ[16] = {};
        struct SN_DOUBLE sn = SN_DOUBLE(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
    } sn_υ4d;
};

struct sn_debugWidget : ModuleWidget {
    sn_debugWidget(sn_debug *);

    void appendContextMenu(Menu *) override;
};
