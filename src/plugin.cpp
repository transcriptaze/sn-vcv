#include "plugin.hpp"

Plugin *pluginInstance;

void init(Plugin *p) {
    pluginInstance = p;

    p->addModel(model_sn_vco);
    p->addModel(model_sn_vcox);
    p->addModel(model_sn_lfo);
    p->addModel(model_sn_lfox);
}
