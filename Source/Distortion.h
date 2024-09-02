/*
  ==============================================================================

    Distortion.h
    Created: 29 Jul 2024 9:15:19pm
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "StateVariableFilter.h"
//#include "PluginProcessor.h"

template <typename SampleType>
class Distortion {
public:
    Distortion();
    enum class DistModel
    {
        kHard,
        kSoft,
        kSat,
        kHard2,
        kFuzz,
        kDiode,
        kLofi,
        kTube
    };

    enum class ParameterId
    {
        kPreamp,
        kThresh,
        kCeiling,
        kMix,
        kOutput,
        kBypass
    };

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setDrive(SampleType newDrive);
    void setGainKnobVal(SampleType newGain);
    void setMix(SampleType newMix);
    void setFilter(SampleType filterfreq);

    void setThresh(SampleType newThresh);
    void setCeiling(SampleType newCeiling);


    void setDistModel(DistModel newDistModel);
    SampleType processSaturation(SampleType inputSample, int channel);
    void process(juce::dsp::ProcessContextReplacing<SampleType>& context) noexcept; 


    SampleType processHardClip2(SampleType dataToClip, bool useDrive, int channel)
    {
        auto wetSignal = dataToClip;
        wetSignal *= gainKnobVal.getNextValue();
        if (useDrive)
        {
            wetSignal *= rawGain.getNextValue();
        }

        auto ceil = ceiling.getNextValue();

        if (std::abs(wetSignal) >= ceil)
        {
            wetSignal *= ceil / std::abs(wetSignal);
        }


        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * .2);

        auto m = (1.0 - mix.getNextValue()) * dataToClip + wetSignal * mix.getNextValue();

        return m;
    }


    SampleType processHardClipper(SampleType inputSample, int channel) {
        
        auto wetSig = inputSample * juce::Decibels::decibelsToGain(input.getNextValue());

        wetSig *= gainKnobVal.getNextValue();
        if (std::abs(wetSig) > 0.50) {
            wetSig *= 0.50 / std::abs(wetSig);
        }

        wetSig *= juce::Decibels::decibelsToGain(-1.0 * .1);

        auto m = (1.0 - mix.getNextValue()) * inputSample + wetSig * mix.getNextValue();

        return m * juce::Decibels::decibelsToGain(1.0);

    }


    SampleType processDiode(SampleType dataToClip, int channel)
    {
        
        auto wetSignal = dataToClip;
        wetSignal *= rawGain.getNextValue();
        wetSignal = processSoftClipper(0.315 * (juce::dsp::FastMathApproximations::exp(0.1 * dataToClip / (_diodeTerm)) - 1.0), false, channel);
        return (1.0 - mix.getNextValue()) * dataToClip + processHardClip2(wetSignal, false, channel)* mix.getNextValue();
    }

    SampleType processTube(SampleType dataToClip, int channel)
    {
        
        auto wetSignal = dataToClip;

        wetSignal *= rawGain.getNextValue();

        if (wetSignal >= 0.0)
        {
            wetSignal = processHardClip2(wetSignal, true, channel);
        }

        else
        {
            wetSignal = processSoftClipper(wetSignal, true, channel);
        }

        wetSignal *= juce::Decibels::decibelsToGain(-gainKnobVal.getNextValue() * 0.23); 

        
        auto m = (1.0 - mix.getNextValue()) * dataToClip + wetSignal * mix.getNextValue();

        return m;
    }

    SampleType processFuzz(SampleType dataToClip, int channel)
    {

        auto wetSignal = dataToClip;

        auto fuzz = processTube(wetSignal, channel);

        wetSignal = processSoftClipper(fuzzFilter.processSample(fuzz, channel), true, channel);

        wetSignal *= 0.5;

        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * 0.2);

        auto m = (1.0 - mix.getNextValue()) * dataToClip + wetSignal * mix.getNextValue();

        return m;
    }


    SampleType processSoftClipper(SampleType dataToClip, bool useDrive, int channel)
    {

        auto wetSignal = dataToClip;
        wetSignal *= gainKnobVal.getNextValue();
        if (useDrive)
        {
            wetSignal *= rawGain.getNextValue();
        }

        wetSignal = _piDivisor * std::atan(wetSignal);

        if (useDrive)
        {
            wetSignal *= 2.0;
            
        }
        wetSignal *= juce::Decibels::decibelsToGain(-gainKnobVal.getNextValue() * 0.2);

        // Mix dry with wet
        auto m = (1.0 - mix.getNextValue()) * dataToClip + wetSignal * mix.getNextValue();

        return m;
    }


    SampleType processSaturationClip(SampleType dataToClip, int channel)
    {
        auto wetSignal = dataToClip;

        auto bias = 0.15;

        wetSignal *= rawGain.getNextValue() + bias;

        auto th = thresh.getNextValue();

        // Saturation algorithim
        if (wetSignal > th)
        {
            wetSignal = th + (wetSignal - th) / (1.0 + std::pow(((wetSignal - 0.5) / th), 2.0));
        }

        wetSignal *= 1.5;

        wetSignal *= juce::Decibels::decibelsToGain(-gainKnobVal.getNextValue());

        // Mix dry with wet
        auto m = (1.0 - mix.getNextValue()) * dataToClip + processSoftClipper(wetSignal - bias, false, channel) * mix.getNextValue();

        return m;
    }


    SampleType processLofi(SampleType dataToClip, int channel)
    {
        // Bias
        auto wetSignal = dataToClip;


        if (wetSignal < 0)
        {
            wetSignal *= juce::jmap(gainKnobVal.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
        }


        wetSignal = processSoftClipper(wetSignal, false, channel);


        wetSignal *= juce::Decibels::decibelsToGain(-gainKnobVal.getNextValue()* 0.2);


        wetSignal = processHardClip2(lofiFilter.processSample(wetSignal, channel), false,  channel);


        auto m = (1.0 - mix.getNextValue()) * dataToClip + wetSignal * mix.getNextValue();

        return m;
    }



private:
    juce::SmoothedValue<float> rawGain;
    juce::SmoothedValue<float> input;
    juce::SmoothedValue<float> mix;
    juce::SmoothedValue<float> thresh;
    juce::SmoothedValue<float> ceiling;
    juce::SmoothedValue<float> lowcut;
    juce::SmoothedValue<float> gainKnobVal;

    static constexpr float _piRaw = juce::MathConstants<float>::pi;
    static constexpr float _diodeTerm = 2.0 * 0.0242;
    static constexpr float _piDivisor = 2.0 / juce::MathConstants<float>::pi;

    float _sampleRate = 44100.0f;


    DistModel model = DistModel::kHard;

    juce::dsp::LinkwitzRileyFilter<float> _dcFilter;
    StateVariableFilter<float> fuzzFilter;
    StateVariableFilter<float> lofiFilter;


};
