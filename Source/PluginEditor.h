/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DistAdvAudioProcessorEditor : public juce::AudioProcessorEditor,
    private juce::Slider::Listener, private juce::Button::Listener,
    public juce::ComboBox::Listener
{
public:
    DistAdvAudioProcessorEditor(DistAdvAudioProcessor&);
    ~DistAdvAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* btn) override;
    DistAdvAudioProcessor& audioProcessor;

    std::unique_ptr<juce::FileChooser> filechooser;

    juce::Slider reverbRoomSizeSlider;
    juce::Slider reverbDampingSlider;
    juce::Slider driveSlider;

    juce::Slider threshSlider;
    juce::Slider mixSlider;
    juce::Slider ceilingSlider;


    juce::Slider tubeMix;
    juce::Slider tubeBias;
    juce::Slider tubeIG;
    juce::Slider tubeOG;
    juce::Slider tubeDrive;



    juce::Slider delayTime;
    juce::Slider delayFeedback;
    juce::Slider delayWet;

    //Dist
    juce::Label lblDistSel;
    juce::Label lblDistDrive;
    juce::Label lblDistCeiling;
    juce::Label lblDistMix;
    juce::Label lblDistThresh;

    //verb
    juce::Label lblRoomSize;
    juce::Label lblDampening;

    //tube ctrls
    juce::Label lbltubeMix;
    juce::Label lbltubeIG;
    juce::Label lbltubeOG;
    juce::Label lbltubeBias;
    juce::Label lbltubeDrive;

    //delay
    juce::Label lbldelayTime;
    juce::Label lbldelayFeedback;
    juce::Label lbldelayWet;

    ScopeComponent<float> scopeComponent;

    juce::TextButton bypassCabSim;
    juce::TextButton bypassVerbBtn;
    juce::TextButton bypassDistBtn;
    juce::TextButton bypassTube;
    juce::TextButton bypassDelay;

    juce::TextButton loadIrBtn;

    juce::ComboBox distSelect;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistAdvAudioProcessorEditor)
};
