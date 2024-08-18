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
}

template <typename SampleType>
void Distortion<SampleType>::prepare(const juce::dsp::ProcessSpec& spec) {
    _sampleRate = spec.sampleRate;

    m_fuzzFilter.prepare(spec);
    m_fuzzFilter.setStereoType(StateVariableFilter<float>::StereoId::kStereo);
    m_fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kType, StateVariableFilter<float>::FilterType::kLowShelf);
    m_fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kQType, StateVariableFilter<float>::QType::kParametric);
    m_fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kCutoff, 2000.0);
    m_fuzzFilter.setParameter(StateVariableFilter<float>::ParameterId::kGain, 9.0);

    m_lofiFilter.prepare(spec);
    m_lofiFilter.setStereoType(StateVariableFilter<float>::StereoId::kStereo);
    m_lofiFilter.setParameter(StateVariableFilter<float>::ParameterId::kType, StateVariableFilter<float>::FilterType::kLowPass);
    m_lofiFilter.setParameter(StateVariableFilter<float>::ParameterId::kQType, StateVariableFilter<float>::QType::kParametric);
    m_lofiFilter.setParameter(StateVariableFilter<float>::ParameterId::kCutoff, 20000.0);



    _dcFilter.prepare(spec);
    _dcFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    
    reset();
}

template <typename SampleType>
void Distortion<SampleType>::reset() {
    if (_sampleRate <= 0) {
        return;
    }
    _rawGain.reset(_sampleRate, 0.02);
    _rawGain.setTargetValue(1.0);

    _input.reset(_sampleRate, 0.02);
    _input.setTargetValue(0.0);

    _mix.reset(_sampleRate, 0.02);
    _mix.setTargetValue(1.0);

    _output.reset(_sampleRate, 0.02);
    _output.setTargetValue(1.0);

    _ceiling.reset(_sampleRate, .02);
    _ceiling.setTargetValue(0.5);

    _thresh.reset(_sampleRate, 0.02);
    _thresh.setTargetValue(1.0);

}

template <typename SampleType>
void Distortion<SampleType>::setDrive(SampleType newDrive) {
    _input.setTargetValue(newDrive);
    _gainDB.setTargetValue(newDrive);
    _rawGain.setTargetValue(juce::Decibels::decibelsToGain(newDrive));
}

template<typename SampleType>
void Distortion<SampleType>::setGainKnobVal(SampleType newGain)
{
    gainKnobVal.setTargetValue(newGain);
}

template <typename SampleType>
void Distortion<SampleType>::setMix(SampleType newMix) {
    _mix.setTargetValue(newMix);
}

template <typename SampleType>
void Distortion<SampleType>::setFilter(SampleType filterfreq) {
    _dcFilter.setCutoffFrequency(filterfreq);
}

template <typename SampleType>
void Distortion<SampleType>::setOutput(SampleType newOutput) {
    _output.setTargetValue(newOutput);
     
}

template <typename SampleType>
void Distortion<SampleType>::setCeiling(SampleType newCeil) {
    _ceiling.setTargetValue(newCeil);

}

template <typename SampleType>
void Distortion<SampleType>::setThresh(SampleType newTresh) {
    _thresh.setTargetValue(newTresh);
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
        case DistModel::kHard2:
        {
            _model = newDistModel;
            break;
        }
        case DistModel::kFuzz:
        {
            _model = newDistModel;
            break;
        }
        case DistModel::kDiode:
        {
            _model = newDistModel;
            break;
        }
        case DistModel::kLofi:
        {
            _model = newDistModel;
            break;
        }
        case DistModel::kTube:
        {
            _model = newDistModel;
            break;
        }

    }
}

template <typename SampleType>
SampleType Distortion<SampleType>::processSaturation(SampleType inputSample, int ch)
{
    switch (_model)
    {
    case DistModel::kHard:
        return processHardClipper(inputSample, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());// implement gain stage
        break;
    case DistModel::kSoft:
        return processSoftClipper(inputSample, true, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
        break;
    case DistModel::kSat:
        return processSaturationClip(inputSample, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
        break;
    case DistModel::kHard2:
        return processHardClip2(inputSample, true, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
        break;
    case DistModel::kTube:
        return processTube(inputSample, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
        break;
    case DistModel::kDiode:
        return processDiode(inputSample, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
        break;
    case DistModel::kLofi:
        return processLofi(inputSample,  ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
        break;
    case DistModel::kFuzz:
        return processFuzz(inputSample, ch) * juce::Decibels::decibelsToGain(_output.getNextValue());
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
            //copy mono to both channels.
            auto* input = inputBlock.getChannelPointer(0);
            auto* output = outputBlock.getChannelPointer(channel);

            output[sample] = processSaturation(_dcFilter.processSample(channel, input[sample]), channel);
        }
    }
}


template class Distortion<float>;
template class Distortion<double>;
