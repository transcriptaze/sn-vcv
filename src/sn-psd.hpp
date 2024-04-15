#include "plugin.hpp"

#include "sn-psd_widget.h"
#include "widgets/aliasing.hpp"

struct sn_psd : Module {
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

    enum STATE {
        COLLECT,
        DFT,
        ESTIMATE,
        IDLE,
    };

    enum FFT_RATE {
        OFF,
        FAST,
        MEDIUM,
        SLOW,
    };

    sn_psd();

    void process(const ProcessArgs &args) override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *) override;

  private:
    void collect(const ProcessArgs &args);
    void dft(const ProcessArgs &args);
    void idle(const ProcessArgs &args);

    void dump();
    void dump_samples();
    void dump_fft();

  public:
    bool enabled = false;

  private:
    STATE state = STATE::IDLE;
    unsigned loops = 0;
    dsp::SchmittTrigger debug;

    struct {
        FFT_RATE rate;
        unsigned ix;
        unsigned samples;
        double buffer[2048];
        bool debug;
    } fft = {
        .ix = 0,
        .samples = 0,
        .buffer = {0.0},
        .debug = false,
    };

    friend struct sn_psd_widget;
};

struct sn_psd_widget : ModuleWidget {
    static const std::vector<std::string> FFT_RATES;

    sn_psd_widget(sn_psd *module);

    void appendContextMenu(Menu *) override;
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
            enabled = module->enabled;
            mode = NONE; // module->antialias;

            snprintf(s, sizeof(s), "%.0f%%", 5.0 * std::round(20.0 * level1));
            text = s;
        }
    }

    sn_psd *module;
};
