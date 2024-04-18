#include <cmath>

#include "psd.hpp"

PSDWidget::PSDWidget() {
}

void PSDWidget::draw(const DrawArgs &args) {
    // ... background
    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, -box.size.x / 2, 0, box.size.x, box.size.y, 2);
    nvgFillColor(args.vg, nvgRGBA(0x19, 0x19, 0x19, 0xff));
    nvgFill(args.vg);

    Widget::draw(args);
}

void PSDWidget::drawLayer(const DrawArgs &args, int layer) {
    if (layer == 1) {
        if (enabled) {
            float v1 = clamp(level1, 0.f, 1.f);
            float v2 = clamp(level2, 0.f, 1.f);
            float h1 = v1 * (box.size.y - 3);
            float h2 = v2 * (box.size.y - 3);

            // ... bar 1
            nvgBeginPath(args.vg);
            nvgRect(args.vg, -6.985 - 5.715, box.size.y - h1 - 2, 5.715, h1);
            nvgFillColor(args.vg, nvgRGB(0xff, 0x00, 0x00));
            nvgFill(args.vg);

            // ... bar 2
            nvgBeginPath(args.vg);
            nvgRect(args.vg, -5.715, box.size.y - h2 - 2, 5.715, h2);
            nvgFillColor(args.vg, nvgRGB(0xff, 0x00, 0x00));
            nvgFill(args.vg);

            // ... options
            nvgBeginPath(args.vg);
            nvgRect(args.vg, 2.445, box.size.y - 4 - 2, 4, 4);
            nvgFillColor(args.vg, mode == ANTIALIAS::X1F1 ? nvgRGB(0xe0, 0xe0, 0x00) : nvgRGB(0x60, 0x60, 0x60));
            nvgFill(args.vg);

            nvgBeginPath(args.vg);
            nvgRect(args.vg, 2.445, box.size.y - 10 - 2, 4, 4);
            nvgFillColor(args.vg, mode == ANTIALIAS::X2F1 ? nvgRGB(0xe0, 0xe0, 0x00) : nvgRGB(0x60, 0x60, 0x60));
            nvgFill(args.vg);

            nvgBeginPath(args.vg);
            nvgRect(args.vg, 2.445, box.size.y - 16 - 2, 4, 4);
            nvgFillColor(args.vg, mode == ANTIALIAS::X4F1 ? nvgRGB(0xe0, 0xe0, 0x00) : nvgRGB(0x60, 0x60, 0x60));
            nvgFill(args.vg);

            nvgBeginPath(args.vg);
            nvgRect(args.vg, 8.16, box.size.y - 4 - 2, 4, 4);
            nvgFillColor(args.vg, mode == ANTIALIAS::X1F2 ? nvgRGB(0xe0, 0xe0, 0x00) : nvgRGB(0x60, 0x60, 0x60));
            nvgFill(args.vg);

            nvgBeginPath(args.vg);
            nvgRect(args.vg, 8.16, box.size.y - 10 - 2, 4, 4);
            nvgFillColor(args.vg, mode == ANTIALIAS::X2F2 ? nvgRGB(0xe0, 0xe0, 0x00) : nvgRGB(0x60, 0x60, 0x60));
            nvgFill(args.vg);

            nvgBeginPath(args.vg);
            nvgRect(args.vg, 8.16, box.size.y - 16 - 2, 4, 4);
            nvgFillColor(args.vg, mode == ANTIALIAS::X4F2 ? nvgRGB(0xe0, 0xe0, 0x00) : nvgRGB(0x60, 0x60, 0x60));
            nvgFill(args.vg);

            // ... text
            prepareFont(args);
            nvgFillColor(args.vg, nvgRGB(0xff, 0xff, 0xff));
            nvgText(args.vg, textPos.x, textPos.y, text.c_str(), NULL);
        } else {
            float h = box.size.y - 3;

            nvgBeginPath(args.vg);
            nvgRect(args.vg, -box.size.x / 2 + 2, box.size.y - h - 2, box.size.x - 4, h);
            nvgFillColor(args.vg, nvgRGB(0x44, 0x44, 0x44));
            nvgFill(args.vg);
        }
    }

    Widget::drawLayer(args, layer);
}

void PSDWidget::step() {
    Widget::step();
}

void PSDWidget::prepareFont(const DrawArgs &args) {
    std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
    if (!font) {
        return;
    }

    nvgFontFaceId(args.vg, font->handle);
    nvgFontSize(args.vg, fontSize);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT);
}
