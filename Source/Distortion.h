/*
  ==============================================================================

    Distortion.h
    Created: 29 Jul 2024 9:15:19pm
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

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
    void setOutput(SampleType newOutput);

    void setThresh(SampleType newThresh);
    void setCeiling(SampleType newCeiling);


    void setDistModel(DistModel newDistModel);
    SampleType processSaturation(SampleType inputSample);
    void process(juce::dsp::ProcessContextReplacing<SampleType>& context) noexcept; 


    /** Hard clip data */
    SampleType processHardClip2(SampleType dataToClip, bool useDrive)
    {
        auto wetSignal = dataToClip;

        if (useDrive)
        {
            wetSignal *= _rawGain.getNextValue();
        }

        auto ceiling = _ceiling.getNextValue();

        // Hard algorithim
        if (std::abs(wetSignal) >= ceiling)
        {
            wetSignal *= ceiling / std::abs(wetSignal);
        }

        // Volume compensation
        //wetSignal *= juce::Decibels::decibelsToGain(-_gainDB.getNextValue() * 1);

        // Mix dry with wet
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }


    SampleType processHardClipper(SampleType inputSample) {

        auto wetSig = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue()) * gainKnobVal.getNextValue();

        if (std::abs(wetSig) > 0.50) {
            wetSig *= 0.50 / std::abs(wetSig);
        }

        auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSig * _mix.getNextValue();

        return mix * juce::Decibels::decibelsToGain(_output.getNextValue());

    }

    /** Diode */
    SampleType processDiode(SampleType dataToClip)
    {
        // Diode algorithim
        auto wetSignal = dataToClip;
        wetSignal *= _rawGain.getNextValue();
        wetSignal = processSoftClipper(0.315 * (juce::dsp::FastMathApproximations::exp(0.1 * dataToClip / (_diodeTerm)) - 1.0), false);
        return (1.0 - _mix.getNextValue()) * dataToClip + processHardClip2(wetSignal, false)* _mix.getNextValue();
    }

    SampleType processTube(SampleType dataToClip)
    {
        // Tube algorithim
        auto wetSignal = dataToClip;

        wetSignal *= _rawGain.getNextValue();

        if (wetSignal >= 0.0)
        {
            wetSignal = processHardClip2(wetSignal, true);
        }

        else
        {
            wetSignal = processSoftClipper(wetSignal, true);
        }

        //wetSignal *= juce::Decibels::decibelsToGain(-_gainDB.getNextValue() * 0.25); 

        // Mix dry with wet
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }
    //wip
    //SampleType processFuzz(SampleType dataToClip, int channel)
    //{
    //    // Fuzz algorithim
    //    auto wetSignal = dataToClip;

    //    auto fuzz = processTube(wetSignal, channel);

    //    wetSignal = softClipData(m_fuzzFilter.processSample(fuzz, channel), true, channel);

    //    wetSignal *= 0.5;

    //    // Mix dry with wet
    //    auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

    //    return mix;
    //}


    SampleType processSoftClipper(SampleType dataToClip, bool useDrive)
    {
        // Soft algorithim
        auto wetSignal = dataToClip;

        if (useDrive)
        {
            wetSignal *= _rawGain.getNextValue();
        }

        wetSignal = _piDivisor * std::atan(wetSignal);

        if (useDrive)
        {
            wetSignal *= 2.0;
            //this makes it too soft, wip
            //wetSignal *= juce::Decibels::decibelsToGain(-_gainDB.getNextValue() * 0.5);
        }

        // Mix dry with wet
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

        return mix;
    }


    SampleType processSaturationClip(SampleType dataToClip)
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

        //wetSignal *= juce::Decibels::decibelsToGain(-_gainDB.getNextValue());

        // Mix dry with wet
        auto mix = (1.0 - _mix.getNextValue()) * dataToClip + processSoftClipper(wetSignal - bias, false) * _mix.getNextValue();

        return mix;
    }

    /** wip */
    //SampleType processLofi(SampleType dataToClip, int channel)
    //{
    //    // Bias
    //    auto wetSignal = dataToClip;

    //    // Lofi algorithim
    //    if (wetSignal < 0)
    //    {
    //        wetSignal *= juce::jmap(_gainDB.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
    //    }

    //    // Saturate signal
    //    wetSignal = processSoftClipper(wetSignal, false, channel);

    //    // Volume compensation
    //    wetSignal *= juce::Decibels::decibelsToGain(_gainDB.getNextValue() * 0.25);

    //    // Over 0 protection
    //    wetSignal = processHardClip2(m_lofiFilter.processSample(wetSignal, channel), false, channel);

    //    // Mix dry with wet
    //    auto mix = (1.0 - _mix.getNextValue()) * dataToClip + wetSignal * _mix.getNextValue();

    //    return mix;
    //}


private:
    juce::SmoothedValue<float> _rawGain;
    juce::SmoothedValue<float> _input;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _output;
    juce::SmoothedValue<float> _gainDB;
    juce::SmoothedValue<float> _thresh;
    juce::SmoothedValue<float> _ceiling;

    juce::SmoothedValue<float> gainKnobVal;

    static constexpr float _piRaw = juce::MathConstants<float>::pi;
    static constexpr float _diodeTerm = 2.0 * 0.0253;
    static constexpr float _piDivisor = 2.0 / juce::MathConstants<float>::pi;

    float _sampleRate = 44100.0f;


    DistModel _model = DistModel::kHard;

    juce::dsp::LinkwitzRileyFilter<float> _dcFilter;


};
