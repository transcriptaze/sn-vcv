#include "sn-vcv.hpp"

void DigitalDisplay::draw(const DrawArgs &args) {
    // ... background
    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 2);
    nvgFillColor(args.vg, nvgRGB(0x19, 0x19, 0x19));
    nvgFill(args.vg);

    prepareFont(args);

    // ... background text
    nvgFillColor(args.vg, bgColor);
    nvgText(args.vg, textPos.x, textPos.y, bgText.c_str(), NULL);
}

void DigitalDisplay::drawLayer(const DrawArgs &args, int layer) {
    if (layer == 1) {
        prepareFont(args);

        // ... foreground text
        nvgFillColor(args.vg, fgColor);
        nvgText(args.vg, textPos.x, textPos.y, text.c_str(), NULL);
    }

    Widget::drawLayer(args, layer);
}

void DigitalDisplay::prepareFont(const DrawArgs &args) {
    std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
    if (!font) {
        return;
    }

    nvgFontFaceId(args.vg, font->handle);
    nvgFontSize(args.vg, fontSize);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgTextAlign(args.vg, NVG_ALIGN_RIGHT);
}
