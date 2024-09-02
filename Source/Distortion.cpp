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
    : _sampleRate(0), model(DistModel::kHard)
{
}

template <typename SampleType>
void Distortion<SampleType>::prepare(const juce::dsp::ProcessSpec& spec) {
    _sampleRate = spec.sampleRate;

    fuzzFilter.prepare(spec);
    fuzzFilter.setStereoType(StateVariableFilter<float>::StereoId::kStereo);
    fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kType, StateVariableFilter<float>::FilterType::kLowShelf);
    fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kQType, StateVariableFilter<float>::QType::kParametric);
    fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kCutoff, 1000.0);
    fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kGain, 9.0);

    lofiFilter.prepare(spec);
    lofiFilter.setStereoType(StateVariableFilter<float>::StereoId::kStereo);
    lofiFilter.setParameter(StateVariableFilter<float>::ParameterId::kType, StateVariableFilter<float>::FilterType::kLowPass);
    lofiFilter.setParameter(StateVariableFilter<float>::ParameterId::kQType, StateVariableFilter<float>::QType::kParametric);
    lofiFilter.setParameter(StateVariableFilter<float>::ParameterId::kCutoff, 10000.0);

    _dcFilter.prepare(spec);
    _dcFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    //_dcFilter.setCutoffFrequency(20.0f);
    
    reset();
}

template <typename SampleType>
void Distortion<SampleType>::reset() {
    if (_sampleRate <= 0) {
        return;
    }
    rawGain.reset(_sampleRate, 0.02);
    rawGain.setTargetValue(1.0);

    lowcut.reset(_sampleRate, 0.02);

    lowcut.setTargetValue(1.0);


    input.reset(_sampleRate, 0.02);
    input.setTargetValue(1.0);

    mix.reset(_sampleRate, 0.02);
    mix.setTargetValue(1.0);

    gainKnobVal.reset(_sampleRate, 0.02);
    gainKnobVal.setTargetValue(1.0);

    ceiling.reset(_sampleRate, .02);
    ceiling.setTargetValue(1.0);

    thresh.reset(_sampleRate, 0.02);
    thresh.setTargetValue(1.0);

}

template <typename SampleType>
void Distortion<SampleType>::setDrive(SampleType newDrive) {
    input.setTargetValue(newDrive);
    rawGain.setTargetValue(juce::Decibels::decibelsToGain(newDrive));
}

template<typename SampleType>
void Distortion<SampleType>::setGainKnobVal(SampleType newGain)
{
    gainKnobVal.setTargetValue(newGain);
}

template <typename SampleType>
void Distortion<SampleType>::setMix(SampleType newMix) {
    mix.setTargetValue(newMix);
}

template <typename SampleType>
void Distortion<SampleType>::setFilter(SampleType filterfreq) {
    _dcFilter.setCutoffFrequency(filterfreq);
}


template <typename SampleType>
void Distortion<SampleType>::setCeiling(SampleType newCeil) {
    ceiling.setTargetValue(newCeil);

}

template <typename SampleType>
void Distortion<SampleType>::setThresh(SampleType newTresh) {
    thresh.setTargetValue(newTresh);
}


template <typename SampleType>
void Distortion<SampleType>::setDistModel(DistModel newDistModel) {
    switch (newDistModel) {
        case DistModel::kHard:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kSoft:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kSat:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kHard2:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kFuzz:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kDiode:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kLofi:
        {
            model = newDistModel;
            break;
        }
        case DistModel::kTube:
        {
            model = newDistModel;
            break;
        }

    }
}

template <typename SampleType>
SampleType Distortion<SampleType>::processSaturation(SampleType inputSample, int ch)
{
    switch (model)
    {
    case DistModel::kHard:
        return processHardClipper(inputSample, ch) * juce::Decibels::decibelsToGain(1.0);// implement gain stage
        break;
    case DistModel::kSoft:
        return processSoftClipper(inputSample, true, ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    case DistModel::kSat:
        return processSaturationClip(inputSample, ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    case DistModel::kHard2:
        return processHardClip2(inputSample, true, ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    case DistModel::kTube:
        return processTube(inputSample, ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    case DistModel::kDiode:
        return processDiode(inputSample, ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    case DistModel::kLofi:
        return processLofi(inputSample,  ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    case DistModel::kFuzz:
        return processFuzz(inputSample, ch) * juce::Decibels::decibelsToGain(1.0);
        break;
    default:
        return inputSample;
    }
}


template <typename SampleType>
void Distortion<SampleType>::process(juce::dsp::ProcessContextReplacing<SampleType>& context) noexcept
{
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    jassert(inputBlock.getNumChannels() == numChannels);
    jassert(inputBlock.getNumSamples() == numSamples);

    if (context.isBypassed)
    {
        outputBlock.copyFrom(inputBlock);
        return;
    }
    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        for (size_t sample = 0; sample < numSamples; ++sample)
        {

            auto* input = inputBlock.getChannelPointer(channel);
            auto* output = outputBlock.getChannelPointer(channel);
            output[sample] = processSaturation(_dcFilter.processSample(channel, input[sample]), channel);

        }
    }
}


template class Distortion<float>;
template class Distortion<double>;
