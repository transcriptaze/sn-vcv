#pragma once

#include "../plugin.hpp"

struct AliasingWidget : Widget {
    AliasingWidget();

    void draw(const DrawArgs &args) override;
    void drawLayer(const DrawArgs &args, int layer) override;
    void step() override;

    float level = 0.f;
};
