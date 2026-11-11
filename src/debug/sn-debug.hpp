#include "../plugin.hpp"

#include <rack.hpp>

struct sn_debug : rack::Module {
    static const int CHANNELS;
    static const float VELOCITY;

    struct iir4 {
        double x[4] = {};
        double y[4] = {};
    };

    enum MODE {
        BASELINE,
        IIR,
        FIR,
        FIR4,
    };

    enum ParamId {
        PARAMS_LEN
    };

    enum InputId {
        IN1_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        OUTA_OUTPUT,
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
    void process_IIR(const ProcessArgs &args);
    void process_FIR(const ProcessArgs &args);
    void process_FIR4(const ProcessArgs &args);

    float butterworth(struct iir4 &iir, double in[4]);

    MODE mode = BASELINE;

    struct {
        double φ = 0.0;
    } baseline;

    struct {
        double φ = 0.0;
        struct iir4 iir;
    } iir;

    struct {
        double φ = 0.0;
        double delay_line[100] = {};
        int head = 0;
    } fir;

    struct {
        double φ = 0.0;
        float state[100] = {};
    } fir4;
};

struct sn_debugWidget : rack::ModuleWidget {
    sn_debugWidget(sn_debug *);

    void appendContextMenu(rack::Menu *) override;
};
