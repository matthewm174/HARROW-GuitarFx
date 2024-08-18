/*
  ==============================================================================

    NoiseGate.h
    Created: 14 Aug 2024 11:14:34pm
    Author:  mattm

  ==============================================================================
*/

#include <JuceHeader.h>

template <typename SampleType>
class NoiseGateEffect
{
public:
    NoiseGateEffect();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::dsp::ProcessContextReplacing<float>& context);
    void setThreshold(SampleType ngThresh);
    void setRatio(SampleType ngRatio);
    void setAttack(SampleType ngAtk);
    void setRelease(SampleType ngRel);

private:
    juce::dsp::NoiseGate<float> noiseGate;
    juce::SmoothedValue<float> _ngThresh;
    juce::SmoothedValue<float> _ngRatio;
    juce::SmoothedValue<float> _ngAtk;
    juce::SmoothedValue<float> _ngRel;
};

