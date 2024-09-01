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
        auto wetSignal = dataToClip;//*gainKnobVal.getNextValue();
        wetSignal *= gainKnobVal.getNextValue();
        if (useDrive)
        {
            wetSignal *= _rawGain.getNextValue();
        }

        auto ceiling = _ceiling.getNextValue();

        if (std::abs(wetSignal) >= ceiling)
        {
            wetSignal *= ceiling / std::abs(wetSignal);
        }


        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * .2);

        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }


    SampleType processHardClipper(SampleType inputSample, int channel) {
        
        auto wetSig = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue());//*gainKnobVal.getNextValue();

        wetSig *= gainKnobVal.getNextValue();
        if (std::abs(wetSig) > 0.50) {
            wetSig *= 0.50 / std::abs(wetSig);
        }

        wetSig *= juce::Decibels::decibelsToGain(-1.0 * .1);

        auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSig * _mix.getNextValue();

        return mix * juce::Decibels::decibelsToGain(1.0);

    }


    SampleType processDiode(SampleType dataToClip, int channel)
    {
        
        auto wetSignal = dataToClip;
        wetSignal *= _rawGain.getNextValue();
        wetSignal = processSoftClipper(0.315 * (juce::dsp::FastMathApproximations::exp(0.1 * dataToClip / (_diodeTerm)) - 1.0), false, channel);
        return (1.0 - _mix.getNextValue()) * dataToClip + processHardClip2(wetSignal, false, channel)* _mix.getNextValue();
    }

    SampleType processTube(SampleType dataToClip, int channel)
    {
        
        auto wetSignal = dataToClip;

        wetSignal *= _rawGain.getNextValue();

        if (wetSignal >= 0.0)
        {
            wetSignal = processHardClip2(wetSignal, true, channel);
        }

        else
        {
            wetSignal = processSoftClipper(wetSignal, true, channel);
        }

        wetSignal *= juce::Decibels::decibelsToGain(-1 * 0.2); 

        
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }

    SampleType processFuzz(SampleType dataToClip, int channel)
    {

        auto wetSignal = dataToClip;

        auto fuzz = processTube(wetSignal, channel);

        wetSignal = processSoftClipper(m_fuzzFilter.processSample(fuzz, channel), true, channel);

        wetSignal *= 0.5;

        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * 0.2);

        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }


    SampleType processSoftClipper(SampleType dataToClip, bool useDrive, int channel)
    {

        auto wetSignal = dataToClip;//*gainKnobVal.getNextValue();
        wetSignal *= gainKnobVal.getNextValue();
        if (useDrive)
        {
            wetSignal *= _rawGain.getNextValue();
        }

        wetSignal = _piDivisor * std::atan(wetSignal);

        if (useDrive)
        {
            wetSignal *= 2.0;
            
        }
        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * 0.2);

        // Mix dry with wet
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }


    SampleType processSaturationClip(SampleType dataToClip, int channel)
    {
        auto wetSignal = dataToClip;

        auto bias = 0.15;

        wetSignal *= _rawGain.getNextValue() + bias;

        auto thresh = _thresh.getNextValue();

        // Saturation algorithim
        if (wetSignal > thresh)
        {
            wetSignal = thresh + (wetSignal - thresh) / (1.0 + std::pow(((wetSignal - 0.5) / thresh), 2.0));
        }

        wetSignal *= 1.5;

        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * 0.2);

        // Mix dry with wet
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + processSoftClipper(wetSignal - bias, false, channel) * _mix.getNextValue();

        return mix;
    }


    SampleType processLofi(SampleType dataToClip, int channel)
    {
        // Bias
        auto wetSignal = dataToClip;


        if (wetSignal < 0)
        {
            wetSignal *= juce::jmap(1.0f, 0.0f, 20.0f, 1.0f, -1.0f);
        }


        wetSignal = processSoftClipper(wetSignal, false, channel);


        wetSignal *= juce::Decibels::decibelsToGain(-1.0 * 0.2);


        wetSignal = processHardClip2(m_lofiFilter.processSample(wetSignal, channel), false,  channel);


        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }



private:
    juce::SmoothedValue<float> _rawGain;
    juce::SmoothedValue<float> _input;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _thresh;
    juce::SmoothedValue<float> _ceiling;
    juce::SmoothedValue<float> _lowcut;
    juce::SmoothedValue<float> gainKnobVal;

    static constexpr float _piRaw = juce::MathConstants<float>::pi;
    static constexpr float _diodeTerm = 2.0 * 0.0253;
    static constexpr float _piDivisor = 2.0 / juce::MathConstants<float>::pi;

    float _sampleRate = 44100.0f;


    DistModel _model = DistModel::kHard;

    juce::dsp::LinkwitzRileyFilter<float> _dcFilter;
    StateVariableFilter<float> m_fuzzFilter;
    StateVariableFilter<float> m_lofiFilter;


};
