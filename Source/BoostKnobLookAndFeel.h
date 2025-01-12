
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class BoostKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BoostKnobLookAndFeel();
    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider) override;

private:
    juce::Image knobImage;
};