#include "NoiseGateEffect.h"

template <typename SampleType>
NoiseGateEffect<SampleType>::NoiseGateEffect()
{
}

template <typename SampleType>
void NoiseGateEffect<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    noiseGate.prepare(spec);
}

template <typename SampleType>
void NoiseGateEffect<SampleType>::reset()
{
    noiseGate.reset();
}

template <typename SampleType>
void NoiseGateEffect<SampleType>::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    noiseGate.process(context);
}

template<typename SampleType>
void NoiseGateEffect<SampleType>::setThreshold(SampleType ngThresh)
{
    noiseGate.setThreshold(ngThresh);
}

template<typename SampleType>
void NoiseGateEffect<SampleType>::setRatio(SampleType ngRatio)
{
    noiseGate.setRatio(ngRatio);
}

template<typename SampleType>
void NoiseGateEffect<SampleType>::setAttack(SampleType ngAtk)
{
    noiseGate.setAttack(ngAtk);
}

template<typename SampleType>
void NoiseGateEffect<SampleType>::setRelease(SampleType ngRel)
{
    noiseGate.setRelease(ngRel);
}


// Explicit template instantiation
template class NoiseGateEffect<float>;
template class NoiseGateEffect<double>;