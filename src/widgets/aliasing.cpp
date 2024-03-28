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
        float v = clamp(level, 0.f, 1.f);
        float h = v * box.size.y;

        nvgBeginPath(args.vg);
        nvgRoundedRect(args.vg, 1, box.size.y - h - 1, box.size.x - 2, h, 2);
        nvgFillColor(args.vg, nvgRGB(0xff, 0x00, 0x00));
        nvgFill(args.vg);
    }

    Widget::drawLayer(args, layer);
}

void AliasingWidget::step() {
    Widget::step();
}
