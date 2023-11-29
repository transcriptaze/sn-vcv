#pragma once

#include <string>
#include <vector>

#include "plugin.hpp"

// ... constants
const float TAU = 2.0f * M_PI; // 2π
const float PI2 = M_PI / 2;    // π/2

// ... control rates
extern const std::vector<unsigned> KRATE;
extern const std::vector<std::string> KRATES;

// ... AUX
extern const float AUX_FREQUENCY;                // auxiliary output fixed frequency (Hz)
extern const std::vector<float> AUX_GAIN;        // auxiliary output default gain
extern const std::vector<std::string> AUX_GAINS; // AUX gain labels
extern const std::vector<std::string> AUX_MODES; // AUX mode labels

enum AUXMODE {
    OSC,
    SUM,
    POLY
};

typedef struct VCO {
    float phase;
    float velocity;
    struct {
        float vco;
        float sum;
    } out;
} VCO;

typedef struct LFO {
    float phase;
    struct {
        float lfo;
        float sum;
    } out;
} LFO;

typedef struct AUX {
    enum AUXMODE mode;
    int gain;
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

    struct Ζ ζ = {
        .pʼ = 1.f,
        .qʼ = 0.f,
        .rʼ = 0.f,
        .sʼ = 1.f,
        .tʼ = 0.f,
        .uʼ = 0.f,
        .φ = 0.f,
    };

    void recompute();
    float υ(float);
    float phi(float, float, float, float);
} SN;

// ... expanders
enum DIRECTION {
    LEFT,
    RIGHT
};

template <typename T>
struct sn_expander {
    sn_expander(DIRECTION direction) {
        this->direction = direction;
        this->module = NULL;
    }

    T *producer() {
        if (module != NULL) {
            switch (direction) {
            case LEFT:
                return (T *)module->getRightExpander().producerMessage;

            case RIGHT:
                return (T *)module->getLeftExpander().producerMessage;
            }
        }

        return NULL;
    }

    void flip() {
        if (module != NULL) {
            switch (direction) {
            case LEFT:
                module->getRightExpander().requestMessageFlip();
                break;

            case RIGHT:
                module->getLeftExpander().requestMessageFlip();
                break;
            }
        }
    }

    DIRECTION direction;
    Module *module;
    bool linked;
    T messages[2];
};

typedef struct sn_vco_message {
    bool linked = false;
    int channels = 1;

    struct _VCO {
        float phase;
        float velocity;
        float out;
    } vco[16] = {
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
        {.phase = 0.f, .velocity = 0.f, .out = 0.f},
    };

    struct _AUX {
        float phase;
        float out;
    } aux = {
        .phase = 0.f,
        .out = 0.f,
    };

    void set(bool linked, int channels, const struct VCO vco[], const struct AUX aux) {
        this->linked = linked;
        this->channels = channels;

        for (int i = 0; i < channels; i++) {
            this->vco[i].phase = vco[i].phase;
            this->vco[i].velocity = vco[i].velocity;
            this->vco[i].out = vco[i].out.sum;
        }

        this->aux.phase = aux.phase;
        this->aux.out = aux.out.sum;
    }
} sn_vco_message;

typedef struct sn_lfo_message {
    bool linked = false;
    int channels = 1;

    struct _LFO {
        float phase;
        float out;
    } lfo[16] = {
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
        {.phase = 0.0f, .out = 0.0f},
    };

    struct _AUX {
        float phase;
        float out;
    } aux = {
        .phase = 0.0f,
        .out = 0.0f,
    };

    void set(bool linked, int channels, const struct LFO lfo[], const struct AUX aux) {
        this->linked = linked;
        this->channels = channels;

        for (int i = 0; i < channels; i++) {
            this->lfo[i].phase = lfo[i].phase;
            this->lfo[i].out = lfo[i].out.sum;
        }

        this->aux.phase = aux.phase;
        this->aux.out = aux.out.sum;
    }
} sn_lfo_message;

// ... channels display widget
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

// ... expander LEDs
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
