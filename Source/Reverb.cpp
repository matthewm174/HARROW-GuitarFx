/*
  ==============================================================================

    Reverb.cpp
    Created: 4 Aug 2024 8:55:59pm
    Author:  mattm

  ==============================================================================
*/

#include "Reverb.h"

template <typename SampleType>
ReverbEffect<SampleType>::ReverbEffect()
{
}

template <typename SampleType>
void ReverbEffect<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    reverb.prepare(spec);
}

template <typename SampleType>
void ReverbEffect<SampleType>::reset()
{
    reverb.reset();
}

template <typename SampleType>
void ReverbEffect<SampleType>::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    //const auto& inputBlock = context.getInputBlock();
    //if (context.isBypassed)
    //{
    //    outputBlock.copyFrom(inputBlock);
    //    return;
    //}
    reverb.process(context);
}

template <typename SampleType>
void ReverbEffect<SampleType>::setParameters(const juce::dsp::Reverb::Parameters& newParams)
{
    reverb.setParameters(newParams);
}

// Explicit template instantiation
template class ReverbEffect<float>;
template class ReverbEffect<double>;