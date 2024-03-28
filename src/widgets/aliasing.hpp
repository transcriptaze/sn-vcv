#pragma once

#include "../plugin.hpp"

struct AliasingWidget : Widget {
    AliasingWidget();

    void draw(const DrawArgs &args) override;
    void drawLayer(const DrawArgs &args, int layer) override;
    void step() override;
    void prepareFont(const DrawArgs &);

    std::string fontPath;
    std::string text;
    float fontSize;
    Vec textPos;

    float level = 0.f;
};
