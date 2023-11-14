#include "sn-vcv.hpp"

// Control rates
const std::vector<unsigned> KRATE = {1, 64, 128, 256};                   // k-rates
const std::vector<std::string> KRATES = {"1", "¹⁄₆₄", "¹⁄₁₂₈", "¹⁄₂₅₆"}; // k-rate labels

// AUX port frequency and mode
const float AUX_FREQUENCY = 25.0;                                  // auxiliary output fixed frequency (Hz)
const std::vector<std::string> AUX_MODES = {"OSC", "SUM", "POLY"}; // AUX mode labels

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
