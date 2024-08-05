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
        kSat
    };

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setDrive(SampleType newDrive);
    void setMix(SampleType newMix);
    void setOutput(SampleType newOutput);
    void setDistModel(DistModel newDistModel);
    SampleType processSaturation(SampleType inputSample);
    void process(juce::dsp::ProcessContextReplacing<SampleType>& context);


    //template <typename ProcessContext>
    //SampleType processSample(SampleType inputSample) noexcept
    //{
    //    switch (_model) {
    //    case DistModel::kHard:
    //    {
    //        return processHardClipper(inputSample);
    //        break;
    //    }
    //    case DistModel::kSoft:
    //    {
    //        return  processSoftClipper(inputSample);
    //        break;
    //    }
    //    case DistModel::kSat:
    //    {
    //        return processSaturation(inputSample);
    //        break;
    //    }
    //    }
    //    //return processHardClipper(inputSample);

    //}

    SampleType processHardClipper(SampleType inputSample) {

        auto wetSig = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue());

        if (std::abs(wetSig) > 0.50) {
            wetSig *= 0.50 / std::abs(wetSig);
        }

        auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSig * _mix.getNextValue();

        return mix * juce::Decibels::decibelsToGain(_output.getNextValue());

    }

    //SampleType processSoftClipper(SampleType inputSample) {
    //    auto wetSig = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue());

    //    wetSig = _piDivisor * std::atan(wetSig);

    //    wetSig *= 2.0;

    //    wetSig *= juce::Decibels::decibelsToGain(_input.getNextValue() * -0.24);

    //    wetSig = juce::dsp::FastMathApproximations::tanh(wetSig);

    //    auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSig * _mix.getNextValue();

    //    return mix * juce::Decibels::decibelsToGain(_output.getNextValue());


    //}

    //SampleType processSaturation(SampleType inputSample) {
    //    auto wetSig = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue());

    //    wetSig = (2.f / _piRaw) * 6 * atan(wetSig);
    //    return wetSig;
    //    //wetSig = _piDivisor * std::tanh(wetSig);

    //    //wetSig *= 2.0;

    //    //wetSig *= juce::Decibels::decibelsToGain(_input.getNextValue() * -0.24);

    //    //wetSig = juce::dsp::FastMathApproximations::tanh(wetSig);

    //    //auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSig * _mix.getNextValue();

    //    //return mix * juce::Decibels::decibelsToGain(_output.getNextValue());

    //}

    //enum class DistModel {
    //    kHard,
    //    kSoft,
    //    kSat
    //};

    //void setDrive(SampleType newDrive);
    //void setMix(SampleType newMix);
    //void setOutput(SampleType newOutput);

    //void setDistModel(DistModel newModel);



private:
    juce::SmoothedValue<float> _input;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _output;
    juce::dsp::LinkwitzRileyFilter<float> _dcFilter;

    float _piRaw = juce::MathConstants<float>::pi;

    float _piDivisor = 2.0 / juce::MathConstants<float>::pi;

    float _sampleRate = 44100.0f;


    DistModel _model = DistModel::kHard;

};