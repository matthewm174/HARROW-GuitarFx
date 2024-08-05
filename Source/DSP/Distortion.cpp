/*
  ==============================================================================

    Distortion.cpp
    Created: 29 Jul 2024 9:15:19pm
    Author:  mattm

  ==============================================================================
*/

#include "Distortion.h"
template <typename SampleType>
Distortion<SampleType>::Distortion()
    : _sampleRate(0), _model(DistModel::kHard)
{
    //processorChain.template get<0>();
}

template <typename SampleType>
void Distortion<SampleType>::prepare(const juce::dsp::ProcessSpec& spec) {
    //_sampleRate = spec.sampleRate;
    //_dcFilter.prepare(spec);

    //_dcFilter.setCutoffFrequency(5.0);
    //_dcFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);

    //reset();
    _sampleRate = spec.sampleRate;
    reset();
}

template <typename SampleType>
void Distortion<SampleType>::reset() {
    if (_sampleRate <= 0) {
        return;
    }

    _input.reset(_sampleRate, 0.02);
    _input.setTargetValue(0.0);

    _mix.reset(_sampleRate, 0.02);
    _mix.setTargetValue(1.0);

    _output.reset(_sampleRate, 0.02);
    _output.setTargetValue(0.0);

}

template <typename SampleType>
void Distortion<SampleType>::setDrive(SampleType newDrive) {
    _input.setTargetValue(newDrive);

}

template <typename SampleType>
void Distortion<SampleType>::setMix(SampleType newMix) {
    _mix.setTargetValue(newMix);
}

template <typename SampleType>
void Distortion<SampleType>::setOutput(SampleType newOutput) {
    _output.setTargetValue(newOutput);
     
}

template <typename SampleType>
void Distortion<SampleType>::setDistModel(DistModel newDistModel) {
    switch (newDistModel) {
        case DistModel::kHard:
        {
            _model = newDistModel;
            break;
        }
        case DistModel::kSoft:
        {
            _model = newDistModel;
            break;
        }
        case DistModel::kSat:
        {
            _model = newDistModel;
            break;
        }

    }
}

template <typename SampleType>
SampleType Distortion<SampleType>::processSaturation(SampleType inputSample)
{
    switch (_model)
    {
    case DistModel::kHard:
        return processHardClipper(inputSample);
        break;

    case DistModel::kSoft:
        return std::tanh(inputSample);

    case DistModel::kSat:
        return inputSample / (1 + std::abs(inputSample));

    default:
        return inputSample;
    }
}

//template <typename SampleType>
//SampleType processHardClipper(SampleType inputSample) {
//
//    auto wetSig = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue());
//
//    if (std::abs(wetSig) > 0.50) {
//        wetSig *= 0.50 / std::abs(wetSig);
//   }
//
//    auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSig * _mix.getNextValue();
//
//    return mix * juce::Decibels::decibelsToGain(_output.getNextValue());
//
//}

template <typename SampleType>
void Distortion<SampleType>::process(juce::dsp::ProcessContextReplacing<SampleType>& context)
{
    auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();

    for (size_t channel = 0; channel < inputBlock.getNumChannels(); ++channel)
    {
        auto* input = inputBlock.getChannelPointer(channel);
        auto* output = outputBlock.getChannelPointer(channel);

        for (size_t sample = 0; sample < inputBlock.getNumSamples(); ++sample)
        {
            output[sample] = processSaturation(input[sample]);
        }
    }
}


template class Distortion<float>;
template class Distortion<double>;
