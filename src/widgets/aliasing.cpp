#include <cmath>

#include "aliasing.hpp"

AliasingWidget::AliasingWidget() {
}

void AliasingWidget::draw(const DrawArgs &args) {
    // ... background
    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 2);
    nvgFillColor(args.vg, nvgRGB(0x19, 0x19, 0x19));
    nvgFill(args.vg);

    Widget::draw(args);
}

void AliasingWidget::drawLayer(const DrawArgs &args, int layer) {
    if (layer == 1) {
        if (enabled) {
            float v = clamp(level, 0.f, 1.f);
            float h = v * (box.size.y - 3);

            // ... bar
            nvgBeginPath(args.vg);
            nvgRect(args.vg, 2, box.size.y - h - 2, 12.7, h);
            nvgFillColor(args.vg, nvgRGB(0xff, 0x00, 0x00));
            nvgFill(args.vg);

            // ... text
            prepareFont(args);
            nvgFillColor(args.vg, nvgRGB(0xff, 0xff, 0xff));
            nvgText(args.vg, textPos.x, textPos.y, text.c_str(), NULL);
        } else {
            float h = box.size.y - 3;

            nvgBeginPath(args.vg);
            nvgRect(args.vg, 2, box.size.y - h - 2, box.size.x - 4, h);
            nvgFillColor(args.vg, nvgRGB(0x00, 0x00, 0x00));
            nvgFill(args.vg);
        }
    }

    Widget::drawLayer(args, layer);
}

void AliasingWidget::step() {
    Widget::step();
}

void AliasingWidget::prepareFont(const DrawArgs &args) {
    std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
    if (!font) {
        return;
    }

    nvgFontFaceId(args.vg, font->handle);
    nvgFontSize(args.vg, fontSize);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT);
}
