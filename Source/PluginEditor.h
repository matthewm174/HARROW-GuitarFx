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
    juce::Slider reverbWidthSlider;
        juce::Slider  reverbWetLevel;
        juce::Slider  reverbDryLevel;
    juce::Slider driveSlider;
    juce::Slider ngPreThreshSlider;
    juce::Slider ngPreRatioSlider;
    juce::Slider ngPreAtkSlider;
    juce::Slider ngPreRelSlider;

    juce::Slider ngPostThreshSlider;
    juce::Slider ngPostRatioSlider;
    juce::Slider ngPostAtkSlider;
    juce::Slider ngPostRelSlider;

    juce::Slider lowpassInSlider;

    juce::Slider threshSlider;
    juce::Slider mixSlider;
    juce::Slider ceilingSlider;

    juce::Slider tubeMix;
    juce::Slider tubeBias;
    juce::Slider tubeIG;
    juce::Slider tubeOG;
    juce::Slider tubeDrive;

    juce::Slider gainKnob;

    juce::Slider delayTime;
    juce::Slider delayFeedback;
    juce::Slider delayWet;

    juce::Label lblNgPreRatio;
    juce::Label lblNgPostRatio;

    juce::Label lblNgPreThreshold;
    juce::Label lblNgPostThreshold;

    juce::Label lblNgPreAtk;
    juce::Label lblNgPostAtk;

    juce::Label lblNgPreRel;
    juce::Label lblNgPostRel;

    //oother
    juce::Label lblMainGain;
    juce::Label lblInputLp;


    //Dist
    juce::Label lblDistSel;
    juce::Label lblDistDrive;
    juce::Label lblDistCeiling;
    juce::Label lblDistMix;
    juce::Label lblDistThresh;

    //verb
    juce::Label lblRoomSize;
    juce::Label lblDampening;
    juce::Label lblRvbWet;
    juce::Label lblRvbDry;
    juce::Label lblRvbWidth;


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
    juce::Label lblTunerNote;
    juce::Label lblTunerOffsetCents;

    juce::String noteName;
    juce::String offsetTune;

    ScopeComponent<float> scopeComponent;

    juce::TextButton bypassCabSim;
    juce::TextButton bypassVerbBtn;
    juce::TextButton bypassDistBtn;
    juce::TextButton bypassTube;
    juce::TextButton bypassDelay;
    juce::TextButton bypassNgPre;
    juce::TextButton bypassNgPost;
    juce::TextButton tunerBtn;

    juce::TextButton loadIrBtn;

    juce::ComboBox distSelect;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistAdvAudioProcessorEditor)
};
