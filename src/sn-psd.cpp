#include "plugin.hpp"

#include "sn-psd.hpp"
#include "sn-psd_widget.h"

sn_psd::sn_psd() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(FREQUENCY_INPUT, "1V/octave pitch");
    configInput(VELOCITY_INPUT, "0-10V velocity");
    configInput(IN_INPUT, "25Hz AUX in");
    configInput(DEBUG_INPUT, "debug");
}

void sn_psd::process(const ProcessArgs &args) {
    if (inputs[IN_INPUT].isConnected() && !connected) {
        connected = true;
        fft.state = COLLECT;
        fft.ix = 0;
    } else if (!inputs[IN_INPUT].isConnected() && connected) {
        connected = false;
        fft.state = IDLE;
    }

    if (!connected) {
        return;
    }

    // ... debug?
    if (debug.processEvent(inputs[DEBUG_INPUT].getVoltage(), 0.f, 1.f) == dsp::TSchmittTrigger<float>::Event::TRIGGERED) {
        fft.debug = true;
    }

    // ... FFT
    if (fft.state == STATE::COLLECT) {
        if (fft.ix < 2048) {
            fft.buffer[fft.ix++] = inputs[IN_INPUT].getVoltage() / 5.f;
        } else {
            fft.state = DFT;
        }
    } else {
        if (fft.debug) {
            fft.debug = false;
            dump();
        }
    }
}

void sn_psd::dump() {
    const char *file = "/tmp/sn-psd-samples.tsv";
    FILE *f = fopen(file, "wt");

    fprintf(f, "i\tsn.υ\n");

    for (size_t i = 0; i < fft.ix; i++) {
        fprintf(f, "%-4lu\t%.5f\n", i, fft.buffer[i]);
    }

    fclose(f);

    INFO(">> sn-vcv::sn-psd dumped sample buffer to %s", file);
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

Model *model_sn_psd = createModel<sn_psd, sn_psd_widget>("sn-psd");