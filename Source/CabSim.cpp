/*
  ==============================================================================

    CabSim.cpp
    Created: 8 Aug 2024 11:06:06pm
    Author:  mattm

  ==============================================================================
*/

#include "CabSim.h"

template <typename Type>
CabSimulator<Type>::CabSimulator()
{
    auto dir = juce::File::getCurrentWorkingDirectory();
    int numTries = 0;

    while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
        dir = dir.getParentDirectory();

    convolution.loadImpulseResponse(dir.getChildFile("Resources").getChildFile("OD-M212-VINT-CND-JP-1-P00-30.wav"),
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::no,
        1024);
}

template <typename Type>
void CabSimulator<Type>::prepare(const juce::dsp::ProcessSpec& spec)
{
    convolution.prepare(spec);
}

template <typename Type>
void CabSimulator<Type>::reset()
{
    convolution.reset();
}

template <typename Type>
void CabSimulator<Type>::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    convolution.process(context);
}

template <typename Type>
void CabSimulator<Type>::changeIr(juce::File inputFileName) {
    convolution.loadImpulseResponse(inputFileName,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
}

template class CabSimulator<float>;
template class CabSimulator<double>;