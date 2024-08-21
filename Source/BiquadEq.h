/*
  ==============================================================================

    BiquadEq.h
    Created: 18 Aug 2024 9:59:48pm
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "BiquadFilter.h"


enum class EqType {
    LPF,
    HPF,
    BPF,
    NOTCH,
    APF,
    PEQ,
    LSH,
    HSH
};
template <typename SampleType>
class BiquadEQ : public BiquadFilter{
    public:
        BiquadEQ();
        void setType(EqType _type);
        void setQ(float _Q);
    void setFreq(float _freq);
    void setGain(float _gain);
    void setEQParams(float _Q, float _freq, float _gain);
    void setSampleRate(float _samplerate) override;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::dsp::ProcessContextReplacing<SampleType>& context) noexcept;


    protected:
    void recalculateCoefficients();

    float Q = 1.0f;
    float freq = 100.f;
    float dbGain = 1.0f;
    float M_PI = 3.14159265359;
    EqType type = EqType::PEQ;
};