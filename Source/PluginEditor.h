/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PresetPanel.h"

class DistAdvAudioProcessorEditor : public juce::AudioProcessorEditor,
    private juce::Slider::Listener, private juce::Button::Listener,
    public juce::ComboBox::Listener
{
public:
    DistAdvAudioProcessorEditor(DistAdvAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~DistAdvAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;



private:

    juce::AudioProcessorValueTreeState& valueTreeState;

    Gui::PresetPanel presetPanel;

    //btn
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tubeOnAtt      ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cabOnAtt       ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> distOnAtt      ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverbOnAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ng2OnAtt       ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ng1OnAtt       ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> delayOnAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tunerOnAtt     ;
    //slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tubeMixAtt      ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tubeBiasAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tubeInAtt       ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tubeOutAtt      ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tubeDriveAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayFeedbackAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayWetAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midEqFreqAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midEqQAtt       ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midEqGainAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiEqFreqAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiEqQAtt        ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiEqGainAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbRoomAtt   ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDampAtt   ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWidthAtt  ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWetAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDryAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAtt        ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveThreshAtt  ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveCeilAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveMixAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPreThreshAtt  ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPreRatioAtt   ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPreAtkAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPreRelAtt     ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPostThreshAtt ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPostRatioAtt  ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPostAtkAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ngPostRelAtt    ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tightenLpAtt    ;

    //    juce::AudioProcessorValueTreeState::ComboBoxAttachment distSelectAtt;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> distSelectAtt;




    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* btn) override;
    DistAdvAudioProcessor& audioProcessor;

    std::unique_ptr<juce::FileChooser> filechooser;

    juce::Slider midEqGainKnob;
    juce::Slider midEqQKnob;
    juce::Slider midEqFreqKnob;
    juce::Slider hiEqGainKnob;
    juce::Slider hiEqQKnob;
    juce::Slider hiEqFreqKnob;
    juce::Slider reverbRoomSizeSlider;
    juce::Slider reverbDampingSlider;
    juce::Slider reverbWidthSlider;
    juce::Slider reverbWetLevel;
    juce::Slider reverbDryLevel;
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

    juce::ToggleButton bypassCabSim;
    juce::ToggleButton bypassVerbBtn;
    juce::ToggleButton bypassDistBtn;
    juce::ToggleButton bypassTube;
    juce::ToggleButton bypassDelay;
    juce::ToggleButton bypassNgPre;
    juce::ToggleButton bypassNgPost;
    juce::ToggleButton tunerBtn;

    juce::ToggleButton loadIrBtn;

    juce::ComboBox distSelect;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistAdvAudioProcessorEditor)
};
