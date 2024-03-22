#include "sn.hpp"

// Control rates
const std::vector<unsigned> KRATE = {1, 64, 128, 256};                   // k-rates
const std::vector<std::string> KRATES = {"1", "¹⁄₆₄", "¹⁄₁₂₈", "¹⁄₂₅₆"}; // k-rate labels

// AUX port
const float AUX_FREQUENCY = 25.0;                                     // AUX output fixed frequency (Hz)
const std::vector<float> AUX_GAIN = {1.f, 0.5f, 0.333f, 0.25f, 0.2f}; // AUX output gain values
const std::vector<std::string> AUX_MODES = {"OSC", "SUM", "POLY"};    // AUX mode labels
const std::vector<std::string> AUX_GAINS = {"1", "½", "⅓", "¼", "⅕"}; // AUX gain labels

// anti-aliasing menu options
const std::vector<std::string> ANTIALIASING = {
    "None",
    "12.5kHz LPF",
    "2x 16kHz LPF",
    "2x oversampling, 16kHz LPF",
    "2x oversampling, 2x 16kHz LPF",
    "4x oversampling, 16kHz LPF",
    "4x oversampling, 2x 16kHz LPF",
};

// DC blocking menu options
const std::vector<std::string> DCBLOCKING = {
    "Off",
    "Slow",
    "Medium",
    "Fast",
};

// SN
void SN::recompute() {
    float εʼ = std::sqrt(1.0f - ε * ε);
    float a = (ε < 0.0f) ? εʼ : 1.0f;
    float b = (ε > 0.0f) ? εʼ : 1.0f;

    float cosθ = std::cos(θ);
    float sinθ = std::sin(θ);

    float u = std::atan(-b * std::tan(θ) / a);
    float v = std::atan((b / std::tan(θ)) * a);
    float tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    float ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    float δxʼ = tx * δx;
    float δyʼ = ty * δy;

    ζ.pʼ = A * a * cosθ;
    ζ.qʼ = A * b * sinθ;
    ζ.rʼ = A * δxʼ;
    ζ.sʼ = A * a * sinθ;
    ζ.tʼ = A * b * cosθ;
    ζ.uʼ = A * δyʼ;
    ζ.φ = phi(a, b, θ, Φ);
}

float SN::υ(float α) {
    float αʼ = m * α - ζ.φ;

    float x = std::cos(αʼ);
    float y = std::sin(αʼ);
    float xʼ = ζ.pʼ * x - ζ.qʼ * y + ζ.rʼ;
    float yʼ = ζ.sʼ * x + ζ.tʼ * y + ζ.uʼ;
    float r = std::hypot(xʼ, yʼ);

    return r > 0.0f ? yʼ / r : 0.0f;
}

float SN::phi(float a, float b, float θ, float Φ) {
    float dθ = Φ - θ;
    float φ = std::atan(-(a / b) * std::tan(Φ - θ));

    if (dθ < -PI2) {
        φ += M_PI;
    } else if (dθ > PI2) {
        φ -= M_PI;
    }

    return φ;
}

// Channels display widget
void ChannelsWidget::draw(const DrawArgs &args) {
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

void ChannelsWidget::drawLayer(const DrawArgs &args, int layer) {
    if (layer == 1) {
        prepareFont(args);

        // ... foreground text
        nvgFillColor(args.vg, fgColor);
        nvgText(args.vg, textPos.x, textPos.y, text.c_str(), NULL);
    }

    Widget::drawLayer(args, layer);
}

void ChannelsWidget::prepareFont(const DrawArgs &args) {
    std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
    if (!font) {
        return;
    }

    nvgFontFaceId(args.vg, font->handle);
    nvgFontSize(args.vg, fontSize);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgTextAlign(args.vg, NVG_ALIGN_RIGHT);
}

// Aliasing widget
AliasingWidget::AliasingWidget() {
    module = NULL;
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
    }

    Widget::drawLayer(args, layer);
}

void AliasingWidget::step() {
    Widget::step();
}
