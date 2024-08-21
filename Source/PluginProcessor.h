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
#include "NoiseGateEffect.h"
#include "BiquadEq.h"
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
    void processTunerSamples(const float* inputSamples, int numSamples);

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

    void setHiFreq(float _freq);

    void setHiQ(float _q);

    void setHiGain(float _freq);




    AudioBufferQueue<float>& getAudioBufferQueue() noexcept { return audioBufferQueue; }

    void setMidFreq(float _freq);

    void setMidQ(float _q);

    void setMidGain(float _freq);

    void setDist(float newDist);
    void setGainKnob(float gain);
    void setMix(float newMix);
    void setCeiling(float newCeil);
    void setThresh(float newThresh);

    void setReverbParameters(const juce::dsp::Reverb::Parameters& params);
    bool bypassReverb();
    bool bypassCab();
    bool bypassDist();
    bool bypassTube();
    bool bypassTuner();
    bool bypassDelay();
    bool bypassNgPre();
    bool bypassNgPost();
    void setCab(juce::File f);
    juce::File root, saveFile;
    void updateParameters(int selection);
    void setTubeDrive(float tubedr);
    void setTubeBias(float tubebias);
    void setTubeMix(float tubemix);
    void setTubeInputGain(float tubeig);
    void setTubeOutputGain(float tubeog);
    void setFilterInFreq(float freq);
    void setDelay(float delaytime);
    void setDelayFb(float delaytime);
    void setDelayWet(float delaytime);


    void setNgPreThresh(float ngpret);
    void setNgPreRatio(float ngprerat);
    void setNgPreAtk(float ngprea);
    void setNgPreRel(float ngprer);


    void setNgPostThresh(float ngpostt);
    void setNgPostRatio(float ngpostrat);
    void setNgPostAtk(float ngposta);
    void setNgPostRel(float ngpostr);
    float tunerfrequency = 0;
    bool tunerOn = false;





private:



    // Static chain of effects, might make this dynamic eventually with graphs
    juce::dsp::ProcessorChain<NoiseGateEffect<float>, Distortion<float>, TubePre<float>, 
        NoiseGateEffect<float>, CabSimulator<float>, BiquadEQ<float>, 
        BiquadEQ<float>, ReverbEffect<float>, DelayEffect<float>> processorChain;

    float  tunerSampleRate;
    int    tunerRecordSize = 2000;
    float  tunerRecordedSamples[2000] = { };
    int    tunerCount = 0;
    float  tunerSum;
    float  tunerSumOld;
    int    tunerPdState = 0;
    int    tunerThresh = 0;




    AudioBufferQueue<float> audioBufferQueue;

    float distLvl = 1.0f;
    juce::dsp::Oscillator<float> oscillator;


    ScopeDataCollector<float> scopeDataCollector{ audioBufferQueue };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistAdvAudioProcessor);


};
