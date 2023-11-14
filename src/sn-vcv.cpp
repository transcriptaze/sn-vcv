#include "sn-vcv.hpp"

// Control rates
const std::vector<unsigned> KRATE = {1, 64, 128, 256};                   // k-rates
const std::vector<std::string> KRATES = {"1", "¹⁄₆₄", "¹⁄₁₂₈", "¹⁄₂₅₆"}; // k-rate labels

// AUX port frequency and mode
const float AUX_FREQUENCY = 25.0;                                  // auxiliary output fixed frequency (Hz)
const std::vector<std::string> AUX_MODES = {"OSC", "SUM", "POLY"}; // AUX mode labels

// SN
void SN::recompute(Χ &χ) {
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

    χ.pʼ = A * a * cosθ;
    χ.qʼ = A * b * sinθ;
    χ.rʼ = A * δxʼ;
    χ.sʼ = A * a * sinθ;
    χ.tʼ = A * b * cosθ;
    χ.uʼ = A * δyʼ;
    χ.φ = phi(a, b, θ, Φ);
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
