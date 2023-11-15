#include "plugin.hpp"

Plugin *pluginInstance;

void init(Plugin *p) {
    pluginInstance = p;

    p->addModel(modelSn_vcv_vco);
    p->addModel(modelSn_vcv_vcox);
}
