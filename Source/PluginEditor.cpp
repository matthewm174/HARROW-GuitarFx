/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Distortion.h"
#include "Oscilloscope.h"
#include <string>
#include "Parameters.h"

//==============================================================================
DistAdvAudioProcessorEditor::DistAdvAudioProcessorEditor
(DistAdvAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), audioProcessor(p), scopeComponent(audioProcessor.getAudioBufferQueue())
{
    setSize(500, 500);
    addAndMakeVisible(scopeComponent);

    gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setRange(1.0, 11.0);
    gainKnob.setValue(1.0);
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainKnob.addListener(this);


    addAndMakeVisible(gainKnob);


    distSelect.addItem("EGGIE", 1);
    distSelect.addItem("SWEETPEA", 4);
    distSelect.addItem("DANDY", 2);
    distSelect.addItem("DAISY", 3);
    distSelect.addListener(this);
    addAndMakeVisible(distSelect);

    driveSlider.setRange(0.0, 80.0);
    driveSlider.setValue(1.0);
    driveSlider.addListener(this);
    addAndMakeVisible(driveSlider);

    ceilingSlider.setRange(0.0, 1.0);
    ceilingSlider.setValue(1);
    ceilingSlider.addListener(this);
    addAndMakeVisible(ceilingSlider);

    mixSlider.setRange(0.0, 1.0);
    mixSlider.setValue(1);
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);

    threshSlider.setRange(0.0, 1.0);
    threshSlider.setValue(1);
    threshSlider.addListener(this);
    addAndMakeVisible(threshSlider);



    reverbRoomSizeSlider.setRange(0.0, 1.0);
    reverbRoomSizeSlider.setValue(0.5);
    reverbRoomSizeSlider.addListener(this);
    addAndMakeVisible(reverbRoomSizeSlider);

    reverbDampingSlider.setRange(0.0, 1.0);
    reverbDampingSlider.setValue(0.5);
    reverbDampingSlider.addListener(this);
    addAndMakeVisible(reverbDampingSlider);


    tubeMix.setRange(1.0, 0.0);
    tubeMix.setValue(1.0);
    tubeMix.addListener(this);
    addAndMakeVisible(tubeMix);

    tubeIG.setRange(0.0, 10.0);
    tubeIG.setValue(1.0);
    tubeIG.addListener(this);
    addAndMakeVisible(tubeIG);

    tubeOG.setRange(0.0, 10.0);
    tubeOG.setValue(1.0);
    tubeOG.addListener(this);
    addAndMakeVisible(tubeOG);

    tubeBias.setRange(-3.0, 3.0);
    tubeBias.setValue(0.0);
    tubeBias.addListener(this);
    addAndMakeVisible(tubeBias);

    tubeDrive.setRange(0.0, 5.0);
    tubeDrive.setValue(1.0);
    tubeDrive.addListener(this);
    addAndMakeVisible(tubeDrive);


    delayTime.setRange(0.0, 3000.0);
    delayTime.setValue(1.0);
    delayTime.addListener(this);
    addAndMakeVisible(delayTime);

    delayFeedback.setRange(0.0, 100.0);
    delayFeedback.setValue(1);
    delayFeedback.addListener(this);
    addAndMakeVisible(delayFeedback);

    delayWet.setRange(0.0, 100.0);
    delayWet.setValue(1);
    delayWet.addListener(this);
    addAndMakeVisible(delayWet);
    //BYPASSING


    //OTHER BTNS
    bypassCabSim.setButtonText("CAB");
    bypassCabSim.addListener(this);
    addAndMakeVisible(bypassCabSim);

    addAndMakeVisible(bypassVerbBtn);
    bypassVerbBtn.setButtonText("VERB");
    bypassVerbBtn.addListener(this);

    addAndMakeVisible(bypassDistBtn);
    bypassDistBtn.setButtonText("DIST");
    bypassDistBtn.addListener(this);

    addAndMakeVisible(bypassTube);
    bypassTube.setButtonText("TUBE");
    bypassTube.addListener(this);

    addAndMakeVisible(loadIrBtn);
    loadIrBtn.setButtonText("LOAD IR");
    loadIrBtn.addListener(this);

    addAndMakeVisible(bypassDelay);
    bypassDelay.setButtonText("DELAY");
    bypassDelay.addListener(this);

    lblDistSel         .setText("Dist Type", juce::dontSendNotification);
    lblDistDrive       .setText("Distortion Level", juce::dontSendNotification);
    lblDistCeiling     .setText("Squash", juce::dontSendNotification);
    lblDistMix         .setText("Distortion Mix", juce::dontSendNotification);
    lblDistThresh      .setText("Slam", juce::dontSendNotification);

    lblRoomSize        .setText("Room Size", juce::dontSendNotification);
    lblDampening       .setText("Dampening", juce::dontSendNotification);

    lbltubeMix         .setText("Tube Mix",juce::dontSendNotification);
    lbltubeIG          .setText("Tube Input Gain",juce::dontSendNotification);
    lbltubeOG          .setText("Tube Output Gain",juce::dontSendNotification);
    lbltubeBias        .setText("Tube Bias",juce::dontSendNotification);
    lbltubeDrive       .setText("Tube Drive",juce::dontSendNotification);
    lbldelayTime       .setText("Delay Time",juce::dontSendNotification);
    lbldelayFeedback   .setText("Delay Feedback",juce::dontSendNotification);
    lbldelayWet        .setText("Delay Mix",juce::dontSendNotification);
    
    juce::Font myfont("Arial", 12.0f, juce::Font::bold);


    lblDistSel         .setFont(myfont);
    lblDistDrive       .setFont(myfont);
    lblDistCeiling     .setFont(myfont);
    lblDistMix         .setFont(myfont);
    lblDistThresh      .setFont(myfont);
    lblRoomSize        .setFont(myfont);
    lblDampening       .setFont(myfont);
    lbltubeMix         .setFont(myfont);
    lbltubeIG          .setFont(myfont);
    lbltubeOG          .setFont(myfont);
    lbltubeBias        .setFont(myfont);
    lbltubeDrive       .setFont(myfont);
    lbldelayTime       .setFont(myfont);
    lbldelayFeedback   .setFont(myfont);
    lbldelayWet        .setFont(myfont);

    lblDistSel        .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistDrive      .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistCeiling    .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistMix        .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistThresh     .setColour(juce::Label::textColourId, juce::Colours::white);
    lblRoomSize       .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDampening      .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeMix        .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeIG         .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeOG         .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeBias       .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeDrive      .setColour(juce::Label::textColourId, juce::Colours::white);
    lbldelayTime      .setColour(juce::Label::textColourId, juce::Colours::white);
    lbldelayFeedback  .setColour(juce::Label::textColourId, juce::Colours::white);
    lbldelayWet       .setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(lblDistSel);
    addAndMakeVisible(lblDistDrive);
    addAndMakeVisible(lblDistCeiling);
    addAndMakeVisible(lblDistMix);
    addAndMakeVisible(lblDistThresh);
    addAndMakeVisible(lblRoomSize);
    addAndMakeVisible(lblDampening);
    addAndMakeVisible(lbltubeMix);
    addAndMakeVisible(lbltubeIG);
    addAndMakeVisible(lbltubeOG);
    addAndMakeVisible(lbltubeBias);
    addAndMakeVisible(lbltubeDrive);
    addAndMakeVisible(lbldelayTime);
    addAndMakeVisible(lbldelayFeedback);
    addAndMakeVisible(lbldelayWet);

}

DistAdvAudioProcessorEditor::~DistAdvAudioProcessorEditor()
{

    reverbDampingSlider.removeListener(this);
    bypassDistBtn.removeListener(this);
    bypassVerbBtn.removeListener(this);
    bypassCabSim.removeListener(this);
    loadIrBtn.removeListener(this);
    reverbRoomSizeSlider.removeListener(this);
    threshSlider.removeListener(this);
    distSelect.removeListener(this);
}


//==============================================================================
void DistAdvAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //
    g.setColour(juce::Colours::white);
    //g.fillAll(juce::Colours::black);
    g.setFont(juce::FontOptions(15.0f));
}

void DistAdvAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    distSelect.setBounds(20, 420, 100, 30);
    gainKnob.setBounds(240, 420, 80, 80);

    scopeComponent.setBounds(0, 0, area.getWidth(), 100);

    reverbRoomSizeSlider.setBounds(20, 120, 300, 30);
    reverbDampingSlider.setBounds(20, 140, 300, 30);

    threshSlider.setBounds(20, 180, 300, 30);
    ceilingSlider.setBounds(20, 200, 300, 30);
    mixSlider.setBounds(20, 220, 300, 30);
    driveSlider.setBounds(20, 160, 300, 30);

    tubeMix.setBounds(20, 240, 300, 30);
    tubeDrive.setBounds(20, 260, 300, 30);
    tubeBias.setBounds(20, 280, 300, 30);
    tubeOG.setBounds(20, 320, 300, 30);
    tubeIG.setBounds(20, 300, 300, 30);

    delayTime.setBounds(20, 340, 300, 30);
    delayFeedback.setBounds(20, 360, 300, 30);
    delayWet.setBounds(20, 380, 300, 30);

    bypassCabSim.setBounds(20, 70, 80, 50);
    bypassVerbBtn.setBounds(100, 70, 80, 50);
    bypassDistBtn.setBounds(180, 70, 80, 50);
    bypassTube.setBounds(340, 70, 80, 50);
    bypassDelay.setBounds(420, 70, 80, 50);

    loadIrBtn.setBounds(260, 70, 70, 50);


    lblDistSel        .setBounds(150, 420, 300, 30);

    lblDistDrive      .setBounds(350, 160, 300, 30);
    lblDistCeiling    .setBounds(350, 200, 300, 30);
    lblDistMix        .setBounds(350, 220, 300, 30);
    lblDistThresh     .setBounds(350, 180, 300, 30);
    lblRoomSize       .setBounds(350, 120, 300, 30);
    lblDampening      .setBounds(350, 140, 300, 30);

    lbltubeMix        .setBounds(350, 240, 300, 30);
    lbltubeIG         .setBounds(350, 300, 300, 30);
    lbltubeOG         .setBounds(350, 320, 300, 30);
    lbltubeBias       .setBounds(350, 280, 300, 30);
    lbltubeDrive      .setBounds(350, 260, 300, 30);

    lbldelayTime      .setBounds(350, 340, 300, 30);
    lbldelayFeedback  .setBounds(350, 360, 300, 30);
    lbldelayWet       .setBounds(350, 380, 300, 30);

}
void DistAdvAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &reverbRoomSizeSlider || slider == &reverbDampingSlider)
    {
        juce::dsp::Reverb::Parameters params;
        params.roomSize = reverbRoomSizeSlider.getValue();
        params.damping = reverbDampingSlider.getValue();
        //params.width = reverb
        audioProcessor.setReverbParameters(params);
    }
    if (slider == &gainKnob) {
        audioProcessor.setGainKnob(gainKnob.getValue());
    }
    if (slider == &driveSlider) {
        audioProcessor.setDist(driveSlider.getValue());
    }
    if (slider == &tubeDrive) {
        audioProcessor.setTubeDrive(tubeDrive.getValue());
    }
    if (slider == &tubeIG) {
        audioProcessor.setTubeInputGain(tubeIG.getValue());
    }
    if (slider == &tubeOG) {
        audioProcessor.setTubeOutputGain(tubeOG.getValue());
    }
    if (slider == &tubeBias) {
        audioProcessor.setTubeBias(tubeBias.getValue());
    }
    if (slider == &tubeMix) {
        audioProcessor.setTubeMix(tubeMix.getValue());
    }
    if (slider == &ceilingSlider) {
        audioProcessor.setCeiling(ceilingSlider.getValue());
    }
    if (slider == &threshSlider) {
        audioProcessor.setThresh(threshSlider.getValue());
    }
    if (slider == &delayTime) {
        audioProcessor.setDelay(delayTime.getValue());
    }
    if (slider == &delayFeedback) {
        audioProcessor.setDelayFb(delayFeedback.getValue());
    }
    if (slider == &delayWet) {
        audioProcessor.setDelayWet(delayWet.getValue());
    }

}

void DistAdvAudioProcessorEditor::buttonClicked(juce::Button* btn) {
    if (btn == &bypassVerbBtn)
    {
        audioProcessor.bypassReverb();
    }
    if (btn == &bypassCabSim)
    {
        audioProcessor.bypassCab();

    }
    if (btn == &bypassDistBtn)
    {
        audioProcessor.bypassDist();

    }
    if (btn == &bypassTube) {
        audioProcessor.bypassTube();
    }
    if (btn == &bypassDelay) {
        audioProcessor.bypassDelay();
    }
    if (btn == &loadIrBtn) {
        filechooser = std::make_unique<juce::FileChooser>("Choose File", audioProcessor.root, "*");

        const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectDirectories;

        filechooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File result(chooser.getResult());
                if (result.getFileExtension() == ".wav") {
                    audioProcessor.saveFile = result;
                    audioProcessor.root = result.getParentDirectory().getFullPathName();
                    audioProcessor.setCab(audioProcessor.saveFile);

                }
            });
    }


}
void DistAdvAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &distSelect)
    {
        int selectedId = distSelect.getSelectedId();
        // Handle the selection change here
        audioProcessor.updateParameters(selectedId);

        juce::Logger::writeToLog("Selected ID: " + juce::String(selectedId));
    }
}