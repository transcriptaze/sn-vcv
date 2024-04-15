#include <algorithm>

#include "plugin.hpp"

#include "sn-psd.hpp"
#include "sn-psd_widget.h"

const std::vector<std::string> sn_psd_widget::FFT_RATES = {
    "Off",
    "100ms",
    "200ms",
    "500ms",
};

sn_psd::sn_psd() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(FREQUENCY_INPUT, "1V/octave pitch");
    configInput(VELOCITY_INPUT, "0-10V velocity");
    configInput(IN_INPUT, "25Hz AUX in");
    configInput(DEBUG_INPUT, "debug");
}

json_t *sn_psd::dataToJson() {
    json_t *root = json_object();

    json_object_set_new(root, "fft-rate", json_integer(fft.rate));

    return root;
}

void sn_psd::dataFromJson(json_t *root) {
    json_t *fft_rate = json_object_get(root, "fft-rate");

    if (fft_rate) {
        switch (json_integer_value(fft_rate)) {
        case FFT_RATE::OFF:
            fft.rate = FFT_RATE::OFF;
            break;

        case FFT_RATE::FAST:
            fft.rate = FFT_RATE::FAST;
            break;

        case FFT_RATE::MEDIUM:
            fft.rate = FFT_RATE::MEDIUM;
            break;

        case FFT_RATE::SLOW:
            fft.rate = FFT_RATE::SLOW;
            break;
        }
    }
}

void sn_psd::process(const ProcessArgs &args) {
    unsigned sampleRate = unsigned(args.sampleRate);

    if (inputs[IN_INPUT].isConnected() && fft.rate != FFT_RATE::OFF && sampleRate >= 44100 && !enabled) {
        loops = 0;
        enabled = true;
        state = COLLECT;

        fft.ix = 0;
        fft.samples = 0;
    } else if ((!inputs[IN_INPUT].isConnected() || fft.rate == FFT_RATE::OFF || sampleRate < 44100) && enabled) {
        enabled = false;
        state = IDLE;
    }

    // ... debug?
    if (debug.processEvent(inputs[DEBUG_INPUT].getVoltage(), 0.f, 1.f) == dsp::TSchmittTrigger<float>::Event::TRIGGERED) {
        INFO(">> sn-vcv::sn-psd DEBUG connected:   %d", inputs[IN_INPUT].isConnected());
        INFO(">> sn-vcv::sn-psd DEBUG rate:        %d", fft.rate);
        INFO(">> sn-vcv::sn-psd DEBUG sample rate: %d", sampleRate);
        INFO(">> sn-vcv::sn-psd DEBUG enabled:     %d", enabled);
        INFO(">> sn-vcv::sn-psd DEBUG state:       %d", state);
        fft.debug = true;
    }

    if (!enabled) {
        return;
    }

    // ... FFT
    if (state == STATE::COLLECT) {
        collect(args);
    } else if (state == STATE::DFT) {
        dft(args);
    } else if (state == STATE::IDLE) {
        if (fft.debug) {
            fft.debug = false;
            dump();
        }

        idle(args);
    }

    loops++;
}

void sn_psd::collect(const ProcessArgs &args) {
    unsigned N = unsigned(args.sampleRate / 25.0);
    unsigned decimate = 1;

    if ((N % 1764) == 0) {
        decimate = int(args.sampleRate) / 44100;
    } else if ((N % 1920) == 0) {
        decimate = int(args.sampleRate) / 48000;
    } else {
        decimate = 1 + (N / 2048);
        N = std::min(unsigned(args.sampleRate / (25.0 * decimate)), unsigned(2048));
    }

    unsigned ix = fft.ix / decimate;

    if (fft.ix < N) {
        if ((fft.ix % decimate) == 0) {
            fft.buffer[ix] = inputs[IN_INPUT].getVoltage() / 5.f;
            fft.samples = ix + 1;
        }

        fft.ix++;
    } else {
        state = STATE::DFT;
    }
}

void sn_psd::dft(const ProcessArgs &args) {
    state = STATE::IDLE;
}

void sn_psd::idle(const ProcessArgs &args) {
    unsigned sampleRate = unsigned(args.sampleRate);
    unsigned N = 100 * sampleRate / 1000;

    switch (fft.rate) {
    case FFT_RATE::OFF:
        N = 500 * sampleRate / 1000;
        break;

    case FFT_RATE::FAST:
        N = 100 * sampleRate / 1000;
        break;

    case FFT_RATE::MEDIUM:
        N = 200 * sampleRate / 1000;
        break;

    case FFT_RATE::SLOW:
        N = 500 * sampleRate / 1000;
        break;
    }

    if (loops > N) {
        fft.ix = 0;
        fft.samples = 0;

        state = STATE::COLLECT;
        loops = 0;
    }
}

void sn_psd::dump() {
    dump_samples();
}

void sn_psd::dump_samples() {
    const char *file = "/tmp/sn-psd-samples.tsv";
    FILE *f = fopen(file, "wt");
    unsigned N = fft.samples;

    fprintf(f, "i\tsn.υ\n");

    for (size_t i = 0; i < N; i++) {
        fprintf(f, "%-4lu\t%.5f\n", i, fft.buffer[i]);
    }

    fclose(f);

    INFO(">> sn-vcv::sn-psd dumped sample buffer to %s", file);
}

void sn_psd::dump_fft() {
}

sn_psd_widget::sn_psd_widget(sn_psd *module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-psd.svg"),
                         asset::plugin(pluginInstance, "res/sn-psd-dark.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addInput(createInputCentered<PJ301MPort>(SN_PSD_INPUT_FREQUENCY, module, sn_psd::FREQUENCY_INPUT));
    addInput(createInputCentered<PJ301MPort>(SN_PSD_INPUT_VELOCITY, module, sn_psd::VELOCITY_INPUT));
    addInput(createInputCentered<PJ301MPort>(SN_PSD_INPUT_IN, module, sn_psd::IN_INPUT));
    addInput(createInputCentered<PJ301MPort>(SN_PSD_INPUT_DEBUG, module, sn_psd::DEBUG_INPUT));

    // ... PSD display
    sn_psd_display *widget = createWidget<sn_psd_display>(SN_PSD_WIDGET_DISPLAY);
    widget->box.size = mm2px(Vec(10.16, 11.43));
    widget->module = module;

    addChild(widget);
}

void sn_psd_widget::appendContextMenu(Menu *menu) {
    sn_psd *module = getModule<sn_psd>();

    menu->addChild(new MenuSeparator);
    menu->addChild(createMenuLabel("sn-psd settings"));

    menu->addChild(createIndexPtrSubmenuItem("FFT rate",
                                             FFT_RATES,
                                             &module->fft.rate));
}

Model *model_sn_psd = createModel<sn_psd, sn_psd_widget>("sn-psd");