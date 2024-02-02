#include "plugin.hpp"

struct sn_lpf : Module {
    enum ParamId {
        PARAMS_LEN
    };
    enum InputId {
        IN_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        OUT_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    sn_lpf() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configInput(IN_INPUT, "in");
        configOutput(OUT_OUTPUT, "out");
    }

    void process(const ProcessArgs &args) override {
        int channels = inputs[IN_INPUT].isConnected() ? inputs[IN_INPUT].getChannels() : 1;

        for (int ch = 0; ch < channels; ch++) {
            float v = inputs[IN_INPUT].getPolyVoltage(ch);

            outputs[OUT_OUTPUT].setVoltage(v);
        }

        outputs[OUT_OUTPUT].setChannels(channels);
    }
};

struct sn_lpfWidget : ModuleWidget {
    sn_lpfWidget(sn_lpf *module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/sn-lpf.svg"),
                             asset::plugin(pluginInstance, "res/sn-lpf-dark.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 20.32)), module, sn_lpf::IN_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.18)), module, sn_lpf::OUT_OUTPUT));
    }
};

Model *model_sn_lpf = createModel<sn_lpf, sn_lpfWidget>("sn-lpf");
