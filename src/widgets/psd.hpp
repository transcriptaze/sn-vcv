#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

struct PSDWidget : Widget {
    PSDWidget();

    void draw(const DrawArgs &args) override;
    void drawLayer(const DrawArgs &args, int layer) override;
    void step() override;
    void prepareFont(const DrawArgs &);

    std::string fontPath;
    std::string text;
    float fontSize = 10;
    Vec textPos = Vec(-7.62, 7.62);
    float level1 = 0.f;
    float level2 = 0.f;
    bool enabled = true;
    ANTIALIAS mode = ANTIALIAS::NONE;
};
