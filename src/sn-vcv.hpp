#pragma once

#include <string>
#include <vector>

#include "plugin.hpp"

// constants
const float TAU = 2.0f * M_PI; // 2π
const float PI2 = M_PI / 2;    // π/2

// Control rates
extern const std::vector<unsigned> KRATE;
extern const std::vector<std::string> KRATES;

// AUX modes
extern const float AUX_FREQUENCY;                // auxiliary output fixed frequency (Hz)
extern const std::vector<std::string> AUX_MODES; // AUX mode labels

enum AUXMODE {
    OSC,
    SUM,
    POLY
};

typedef struct AUX {
    enum AUXMODE mode;
    float phase;
    struct {
        float osc;
        float sum;
    } out;
} AUX;

// SN
typedef struct Ζ {
    float pʼ;
    float qʼ;
    float rʼ;
    float sʼ;
    float tʼ;
    float uʼ;
    float φ;
} Ζ;

typedef struct SN {
    SN(float ε, float θ, float A, float δx, float δy, float Φ, float m) {
        this->ε = ε;
        this->θ = θ;
        this->A = A;
        this->δx = δx;
        this->δy = δy;
        this->Φ = Φ;
        this->m = m;
    }

    float ε;
    float θ;
    float A;
    float δx;
    float δy;
    float Φ;
    float m;

    void recompute(Ζ &);
    float phi(float, float, float, float);
} SN;

// Channels display widget
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

// Expander LEDs
static const NVGcolor SCHEME_DARK_GREEN = nvgRGB(0x00, 0xa0, 0x00);
static const NVGcolor SCHEME_BRIGHT_RED = nvgRGB(0xff, 0x00, 0x00);

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
