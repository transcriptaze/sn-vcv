#pragma once

#include <string>
#include <vector>

#include "plugin.hpp"

static const NVGcolor SCHEME_DARK_GREEN = nvgRGB(0x00, 0xa0, 0x00);
static const NVGcolor SCHEME_BRIGHT_RED = nvgRGB(0xff, 0x00, 0x00);

enum AUXMODE {
    OSC,
    SUM,
    POLY
};

struct DigitalDisplay : Widget {
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

template <typename TBase>
struct XLeftLight : TBase {
    XLeftLight() {
        this->box.size = mm2px(math::Vec(2.54, 2.54));
    }

    void drawBackground(const widget::Widget::DrawArgs &args) override {
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, 0, this->box.size.y / 2.0);
        nvgLineTo(args.vg, this->box.size.x, this->box.size.y);
        nvgLineTo(args.vg, this->box.size.x, 0.0);
        nvgClosePath(args.vg);

        NVGcolor bg = this->bgColor;
        NVGcolor border = this->borderColor;

        bg.a = this->color.a;
        border.a = this->color.a;

        if (bg.a > 0.0) {
            nvgFillColor(args.vg, bg);
            nvgFill(args.vg);
        }

        if (border.a > 0.0) {
            nvgStrokeWidth(args.vg, 0.5);
            nvgStrokeColor(args.vg, border);
            nvgStroke(args.vg);
        }
    }

    void drawLight(const widget::Widget::DrawArgs &args) override {
        if (this->color.a > 0.0) {
            nvgBeginPath(args.vg);
            nvgMoveTo(args.vg, 0, this->box.size.y / 2.0);
            nvgLineTo(args.vg, this->box.size.x, this->box.size.y);
            nvgLineTo(args.vg, this->box.size.x, 0.0);
            nvgClosePath(args.vg);

            nvgFillColor(args.vg, this->color);
            nvgFill(args.vg);
        }
    }
};

template <typename TBase>
struct XRightLight : TBase {
    XRightLight() {
        this->box.size = mm2px(math::Vec(2.54, 2.54));
    }

    void drawBackground(const widget::Widget::DrawArgs &args) override {
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, this->box.size.x, this->box.size.y / 2.0);
        nvgLineTo(args.vg, 0.0, this->box.size.y);
        nvgLineTo(args.vg, 0.0, 0.0);
        nvgClosePath(args.vg);

        NVGcolor bg = this->bgColor;
        NVGcolor border = this->borderColor;

        bg.a = this->color.a;
        border.a = this->color.a;

        if (bg.a > 0.0) {
            nvgFillColor(args.vg, bg);
            nvgFill(args.vg);
        }

        if (border.a > 0.0) {
            nvgStrokeWidth(args.vg, 0.5);
            nvgStrokeColor(args.vg, border);
            nvgStroke(args.vg);
        }
    }

    void drawLight(const widget::Widget::DrawArgs &args) override {
        if (this->color.a > 0.0) {
            nvgBeginPath(args.vg);
            nvgMoveTo(args.vg, this->box.size.x, this->box.size.y / 2.0);
            nvgLineTo(args.vg, 0.0, this->box.size.y);
            nvgLineTo(args.vg, 0.0, 0.0);
            nvgClosePath(args.vg);

            nvgFillColor(args.vg, this->color);
            nvgFill(args.vg);
        }
    }
};

template <typename TBase = GrayModuleLightWidget>
struct TDarkGreenLight : TBase {
    TDarkGreenLight() {
        this->addBaseColor(SCHEME_DARK_GREEN);
    }
};

using DarkGreenLight = TDarkGreenLight<>;

template <typename TBase = GrayModuleLightWidget>
struct TBrightRedLight : TBase {
    TBrightRedLight() {
        this->addBaseColor(SCHEME_BRIGHT_RED);
    }
};

using BrightRedLight = TBrightRedLight<>;
