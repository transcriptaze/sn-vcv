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
extern const std::vector<float> AUX_GAIN;        // auxiliary output gains
extern const std::vector<std::string> AUX_GAINS; // AUX gain labels
extern const std::vector<std::string> AUX_MODES; // AUX mode labels

// ... anti-aliasing
extern const std::vector<std::string> ANTIALIASING; // anti-aliasing option labels

// ... DC blocking
extern const std::vector<std::string> DCBLOCKING; // DC blocking option labels

// ... aliasing
extern const std::vector<std::string> ALIASING; // aliasing widget update option labels

enum AUXMODE {
    OSC,
    SUM,
    POLY
};

enum ANTIALIAS {
    NONE,
    X1F1,
    X1F2,
    X2F1,
    X2F2,
    X4F1,
    X4F2,
};

enum DCBLOCK {
    DCBLOCK_NONE,
    DCBLOCK_SLOW,
    DCBLOCK_MEDIUM,
    DCBLOCK_FAST,
};

typedef struct VCO {
    float α;
    float velocity;
    float phase[4];
    struct {
        float vco[4];
        float sum[4];
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
    ANTIALIAS antialias;
    DCBLOCK dcblocking;

    struct _VCO {
        float velocity;
        float phase[4];
        float out[4];
    } vco[16] = {
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
        {.velocity = 0.f, .phase = {0.f, 0.f, 0.f, 0.f}, .out = {0.f, 0.f, 0.f, 0.f}},
    };

    struct _AUX {
        float phase;
        float out;
    } aux = {
        .phase = 0.f,
        .out = 0.f,
    };

    void set(bool linked, int channels, ANTIALIAS antialias, DCBLOCK dcblocking, const struct VCO vco[], const struct AUX aux) {
        this->linked = linked;
        this->channels = channels;
        this->antialias = antialias;
        this->dcblocking = dcblocking;

        for (int i = 0; i < channels; i++) {
            this->vco[i].velocity = vco[i].velocity;
            for (int j = 0; j < 4; j++) {
                this->vco[i].phase[j] = vco[i].phase[j];
                this->vco[i].out[j] = vco[i].out.sum[j];
            }
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
