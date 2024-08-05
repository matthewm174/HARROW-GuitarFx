#ifndef REVERBEFFECT_H_INCLUDED
#define REVERBEFFECT_H_INCLUDED

#include <JuceHeader.h>

template <typename SampleType>
class ReverbEffect
{
public:
    ReverbEffect();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::dsp::ProcessContextReplacing<float>& context);
    void setParameters(const juce::dsp::Reverb::Parameters& newParams);

private:
    juce::dsp::Reverb reverb;
};

#endif // REVERBEFFECT_H_INCLUDED