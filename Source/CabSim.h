/*
  ==============================================================================

    CabSim.h
    Created: 4 Aug 2024 5:04:24pm
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename Type>
class CabSimulator
{
public:
    CabSimulator();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::dsp::ProcessContextReplacing<float>& context);
    void changeIr(juce::File inputFileName);
private:

    juce::dsp::ProcessorChain<juce::dsp::Convolution> cabProcChain;

    juce::dsp::Convolution convolution;
};