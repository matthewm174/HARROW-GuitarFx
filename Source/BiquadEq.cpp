/*
  ==============================================================================

    BiquadEq.cpp
    Created: 18 Aug 2024 9:59:48pm
    Author:  mattm

  ==============================================================================
*/

#include "BiquadEq.h"
#include <cmath>

template<typename SampleType>
BiquadEQ<SampleType>::BiquadEQ()
{
}

template <typename SampleType>
void BiquadEQ <typename SampleType>::setType(EqType _type) {
    type = _type;
    recalculateCoefficients();
}

template <typename SampleType>
void BiquadEQ <typename SampleType>::setQ(float _Q){
    Q = _Q;
    recalculateCoefficients();
}
template <typename SampleType>
void BiquadEQ <typename SampleType>::setFreq(float _freq){
    freq = _freq;
    recalculateCoefficients();
}
template <typename SampleType>
void BiquadEQ<typename SampleType>::setGain(float _gain){
    dbGain = _gain;
    recalculateCoefficients();
}
template <typename SampleType>
void BiquadEQ<typename SampleType>::setEQParams(float _Q, float _freq, float _gain){
    Q = _Q;
    freq = _freq;
    dbGain = _gain;
    recalculateCoefficients();
}
template <typename SampleType>
void BiquadEQ<typename SampleType>::recalculateCoefficients() {
    float omega = 2.0 * M_PI * freq / sampleRate;
    float alpha = sin(freq * 2. * M_PI * oneOverSamplerate) / (2.f * Q);
    float A = pow(10, dbGain / 40);
    float a0 = 1. + alpha / A;
    float sinOmega = std::sin(omega);
    float cosOmega = std::cos(omega);
    switch (type) {
        case EqType::PEQ:
            b0 = (1. + alpha * A);
            b1 = -2. * cos(freq * 2. * M_PI * oneOverSamplerate);
            b2 = (1 - alpha * A);
            a1 = b1;// these are calculated the same
            a2 = (1 - alpha / A);
            break;

        case EqType::HPF:
            b0 = (1.0 + cosOmega) / 2.0;
            b1 = -(1.0 + cosOmega);
            b2 = (1.0 + cosOmega) / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alpha;
            break;
        case EqType::LPF:
            b0 = (1.0 - cosOmega) / 2.0;
            b1 = 1.0 - cosOmega;
            b2 = (1.0 - cosOmega) / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosOmega;
            a2 = 1.0 - alpha;
            break;
        default:
            b0 = (1. + alpha * A);
            b1 = -2. * cos(freq * 2. * M_PI * oneOverSamplerate);
            b2 = (1 - alpha * A);
            a1 = b1;// these are calculated the same
            a2 = (1 - alpha / A);
            break;
    }

    //normalize
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;


}
template <typename SampleType>
void BiquadEQ<typename SampleType>::setSampleRate(float _samplerate) {
    BiquadFilter::setSampleRate(_samplerate);
    recalculateCoefficients();
}

template<typename SampleType>
void BiquadEQ<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    oneOverSamplerate = 1.0f / spec.sampleRate;
    reset();
}

template<typename SampleType>
void BiquadEQ<SampleType>::reset()
{
}

template<typename SampleType>
void BiquadEQ<SampleType>::process(juce::dsp::ProcessContextReplacing<SampleType>& context) noexcept
{

    auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();

    int numSamples = outputBlock.getNumSamples();
    auto* channelDataL = outputBlock.getChannelPointer(0);
    auto* channelDataR = outputBlock.getChannelPointer(1);

    for (int i = 0; i < numSamples; i++)
    {
        channelDataL[i] = BiquadFilter::doFilter(channelDataL[i]);
        channelDataR[i] = BiquadFilter::doFilter(channelDataR[i]);
    }

}


template class BiquadEQ<float>;
template class BiquadEQ<double>;