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
    //==============================================================================
    CabSimulator()
    {
        auto dir = juce::File::getCurrentWorkingDirectory();
        int numTries = 0;

        while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
            dir = dir.getParentDirectory();

        convolution.loadImpulseResponse(dir.getChildFile("Resources").getChildFile("guitar_amp.wav"),
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::no,
            1024);
    }

    //==============================================================================
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        convolution.prepare(spec);
    }

    //==============================================================================
    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        convolution.process(context);
    }

    //==============================================================================
    void reset() noexcept
    {
        convolution.reset();
    }

private:
    //==============================================================================
    //enum
    //{
    //    convolutionIndex
    //};
    juce::dsp::Convolution convolution;

    
};