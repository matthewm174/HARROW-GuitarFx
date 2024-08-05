/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/Distortion.h"
#include "DSP/CabSim.h"
#include "DSP/Oscilloscope.h"
#include "DSP/Reverb.h"
//==============================================================================
/**
*/
class DistAdvAudioProcessor  : public juce::AudioProcessor, 
    juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DistAdvAudioProcessor();
    ~DistAdvAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    juce::AudioProcessorValueTreeState& getAudioProcessorValueTreeState();

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState _treeState;

    AudioBufferQueue<float>& getAudioBufferQueue() noexcept { return audioBufferQueue; }
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void setDist(float newDist);
    void setReverbParameters(const juce::dsp::Reverb::Parameters& params);


private:
    //==============================================================================
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void updateParameters();


    using DistortionProcessor = juce::dsp::ProcessorChain<Distortion<float>, CabSimulator<float>, ReverbEffect<float>>;

    DistortionProcessor processorChain;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAudioProcessor)
    AudioBufferQueue<float> audioBufferQueue;

    float distLvl = 1.0f;


    ScopeDataCollector<float> scopeDataCollector{ audioBufferQueue };

    //Distortion<float> _distortionModule;

    //juce::dsp::ProcessorChain<Distortion<float>, CabSimulator<float>> fxChain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistAdvAudioProcessor);
};
