/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Distortion.h"
#include "CabSim.h"
#include "Oscilloscope.h"
#include "Reverb.h"
#include "TubePre.h"
#include "Delay.h"
//==============================================================================

class DistAdvAudioProcessor 
    : public juce::AudioProcessor
{
public:


    DistAdvAudioProcessor();
    ~DistAdvAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    AudioBufferQueue<float>& getAudioBufferQueue() noexcept { return audioBufferQueue; }
    void setDist(float newDist);
    void setGainKnob(float gain);
    void setMix(float newMix);
    void setCeiling(float newCeil);
    void setThresh(float newThresh);

    void setReverbParameters(const juce::dsp::Reverb::Parameters& params);
    void bypassReverb();
    void bypassCab();
    void bypassDist();
    void bypassTube();
    void bypassDelay();
    void setCab(juce::File f);
    juce::File root, saveFile;
    void updateParameters(int selection);
    void setTubeDrive(float tubedr);
    void setTubeBias(float tubedr);
    void setTubeMix(float tubedr);
    void setTubeInputGain(float tubedr);
    void setTubeOutputGain(float tubedr);
    void setDelay(float delaytime);
    void setDelayFb(float delaytime);
    void setDelayWet(float delaytime);


private:
    // Static chain of effects, might makek this dynamic eventually with graphs
    juce::dsp::ProcessorChain<Distortion<float>, TubePre<float>, CabSimulator<float>, ReverbEffect<float>, DelayEffect<float> > processorChain;

    AudioBufferQueue<float> audioBufferQueue;

    float distLvl = 1.0f;
    juce::dsp::Oscillator<float> oscillator;


    ScopeDataCollector<float> scopeDataCollector{ audioBufferQueue };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistAdvAudioProcessor);


};
