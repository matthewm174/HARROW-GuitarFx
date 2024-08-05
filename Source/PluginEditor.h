/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistAdvAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    DistAdvAudioProcessorEditor (DistAdvAudioProcessor&);
    ~DistAdvAudioProcessorEditor() override;

    //===================================   ===========================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistAdvAudioProcessor& audioProcessor;

    juce::Slider reverbRoomSizeSlider;
    juce::Slider reverbDampingSlider;

    //void verbSliderChanged(juce::Slider* slider) override;


    juce::Slider driveSlider;
    ScopeComponent<float> scopeComponent;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistAdvAudioProcessorEditor)
};
