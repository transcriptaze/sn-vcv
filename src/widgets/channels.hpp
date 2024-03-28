#pragma once

#include "../plugin.hpp"

struct ChannelsWidget : Widget {
    void draw(const DrawArgs &args) override;
    void drawLayer(const DrawArgs &args, int layer) override;
    void prepareFont(const DrawArgs &);

    std::string fontPath;
    std::string bgText;
    std::string text;
    float fontSize;
    NVGcolor bgColor = nvgRGB(0x46, 0x46, 0x46);
    NVGcolor fgColor = SCHEME_YELLOW;
    Vec textPos;
};
