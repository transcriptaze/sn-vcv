#include "../plugin.hpp"

#include <rack.hpp>

struct sn_debug : rack::Module {
    static const int CHANNELS;
    static const float VELOCITY;

    enum MODE {
        BASELINE,
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
    void process_FIR4(const ProcessArgs &args);

    MODE mode = BASELINE;

    struct {
        double φ = 0.0;
    } baseline;

    struct {
        double φ = 0.0;
        float state[100] = {};
    } fir4;
};

struct sn_debugWidget : rack::ModuleWidget {
    sn_debugWidget(sn_debug *);

    void appendContextMenu(rack::Menu *) override;
};
