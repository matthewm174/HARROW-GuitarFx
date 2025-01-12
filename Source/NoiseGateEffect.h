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

};

