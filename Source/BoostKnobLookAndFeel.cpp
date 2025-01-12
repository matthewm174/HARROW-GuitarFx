
#include "BoostKnobLookAndFeel.h"
#include "BinaryData.h"

BoostKnobLookAndFeel::BoostKnobLookAndFeel()
{
    knobImage = juce::ImageFileFormat::loadFrom(BinaryData::boost_knob_png, BinaryData::boost_knob_pngSize);
}

void BoostKnobLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    if (knobImage.isValid())
    {
        const int numFrames = knobImage.getHeight() / knobImage.getWidth();
        const int frameIndex = (int)std::round(sliderPosProportional * (numFrames - 1)) % numFrames;

        const int frameHeight = knobImage.getHeight() / numFrames;
        const int frameWidth = knobImage.getWidth();

        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;

        juce::Rectangle<float> knobBounds(centerX - frameWidth * 0.5f,
            centerY - frameHeight * 0.5f,
            (float)frameWidth,
            (float)frameHeight);

        juce::RectanglePlacement rect;
        rect = juce::RectanglePlacement();

        g.drawImage(knobImage, knobBounds, rect);
    }
    else
    {
        juce::LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, slider);
    }
}