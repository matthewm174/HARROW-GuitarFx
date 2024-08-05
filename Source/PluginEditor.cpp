/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/Distortion.h"
#include "DSP/Oscilloscope.h"
#include <string>
#include "Parameters/Parameters.h"

//==============================================================================
DistAdvAudioProcessorEditor::DistAdvAudioProcessorEditor 
(DistAdvAudioProcessor& p)
: juce::AudioProcessorEditor (&p), audioProcessor (p), scopeComponent(audioProcessor.getAudioBufferQueue())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //addAndMakeVisible(audioProcessor);
    setSize(400, 500);
    //std::string stdStr = p.hasEditor() ? "true" : "false";
    //DBG(stdStr);
    driveSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    //driveSlider.
    driveSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 0);
    driveSlider.setRange(0.0, 24.0, .05);
    driveSlider.setValue(1.0);
    driveSlider.addListener(this);
    addAndMakeVisible(&driveSlider);
    
    addAndMakeVisible(scopeComponent);


    reverbRoomSizeSlider.setRange(0.0, 1.0);
    reverbRoomSizeSlider.setValue(0.5);
    reverbRoomSizeSlider.addListener(this);
    addAndMakeVisible(reverbRoomSizeSlider);

    reverbDampingSlider.setRange(0.0, 1.0);
    reverbDampingSlider.setValue(0.5);
    reverbDampingSlider.addListener(this);
    addAndMakeVisible(reverbDampingSlider);




    //auto& apvts = audioProcessor.getAudioProcessorValueTreeState();
    //driveSlider.setValue(*apvts.getRawParameterValue(inputID));

    auto area = getLocalBounds();
    scopeComponent.setTopLeftPosition(0, 0);
    scopeComponent.setSize(area.getWidth()/2, area.getHeight()/2);
}

DistAdvAudioProcessorEditor::~DistAdvAudioProcessorEditor()
{
}


//==============================================================================
void DistAdvAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //
    g.setColour (juce::Colours::white);
    //g.fillAll(juce::Colours::black);
    g.setFont (juce::FontOptions (15.0f));
}

void DistAdvAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    //scopeComponent.setBounds(area);

    reverbRoomSizeSlider.setBounds(10, 50, getWidth() - 20, 30);
    reverbDampingSlider.setBounds(10, 90, getWidth() - 20, 30);

    driveSlider.setBounds(200, 30, 20, getHeight() - 60);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
void DistAdvAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &reverbRoomSizeSlider || slider == &reverbDampingSlider)
    {
        juce::dsp::Reverb::Parameters params;
        params.roomSize = reverbRoomSizeSlider.getValue();
        params.damping = reverbDampingSlider.getValue();
        audioProcessor.setReverbParameters(params);
    }

    if (slider == &driveSlider) {
        audioProcessor.setDist(driveSlider.getValue());
    }

}
