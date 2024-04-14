#include "plugin.hpp"

#include "sn-psd_widget.h"
#include "widgets/aliasing.hpp"

struct sn_psd : Module {
    enum STATE {
        COLLECT,
        DFT,
        ESTIMATE,
        IDLE,
    };

    enum ParamId {
        PARAMS_LEN
    };
    enum InputId {
        FREQUENCY_INPUT,
        VELOCITY_INPUT,
        IN_INPUT,
        DEBUG_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    sn_psd();

    void process(const ProcessArgs &args) override;

  private:
    void dump(unsigned decimate);

  public:
    bool connected = false;
    dsp::SchmittTrigger debug;

  private:
    struct {
        STATE state;
        unsigned ix;
        double buffer[2048];
        bool debug;
    } fft = {
        .state = IDLE,
        .ix = 0,
        .buffer = {0.0},
        .debug = false,
    };
};

struct sn_psd_widget : ModuleWidget {
    sn_psd_widget(sn_psd *module);
};

struct sn_psd_display : AliasingWidget {
    sn_psd_display() {
        module = NULL;

        fontPath = asset::system("res/fonts/Nunito-Bold.ttf");
        text = "0.00";
    }

    void step() override {
        Widget::step();

        if (module != NULL) {
            char s[16];

            level1 = 0.0; // clamp(module->fft.q, 0.f, 1.f);
            level2 = 0.0; // clamp(module->fft.q, 0.f, 1.f);
            enabled = module->connected;
            mode = NONE; // module->antialias;

            snprintf(s, sizeof(s), "%.0f%%", 5.0 * std::round(20.0 * level1));
            text = s;
        }
    }

    sn_psd *module;
};
