/*
  ==============================================================================

    ParameterState.h
    Created: 21 Aug 2024 8:52:55pm
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Utility
{
	class ParameterHelper
	{
	public:
		ParameterHelper() = delete;
		//Main Component Parameter Tree
		static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
		{
			return juce::AudioProcessorValueTreeState::ParameterLayout{
				std::make_unique<juce::AudioParameterFloat>("gain", "Gain",                         juce::NormalisableRange<float>{0.f, 11.0f, 0.1f}, 1.f),//    ngPreRatioSlider.setRange(1.0f, 10.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("tubeMix", "Tube Mix",                  juce::NormalisableRange<float>{0.f, 1.0f, 0.01f}, 1.f),//    ngPreThreshSlider.setRange(-30.0f, 30.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("tubeBias", "Tube Bias",                juce::NormalisableRange<float>{0.f, 2.0f, 0.1f}, 1.f),//    ngPreAtkSlider.setRange(5.0f, 1000.0f, 5.0f);
				std::make_unique<juce::AudioParameterFloat>("tubeIn", "Tube Input",                 juce::NormalisableRange<float>{0.f, 2.0f, 0.1f}, 1.f),//    ngPreRelSlider.setRange(5.0f, 1000.0f, 5.0f);
				std::make_unique<juce::AudioParameterFloat>("tubeOut", "Tube Output",               juce::NormalisableRange<float>{0.f, 3.0f, 0.1f}, 1.f),//    ngPostThreshSlider.setRange(-30.0f, 30.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("tubeDrive", "Tube Drive",              juce::NormalisableRange<float>{0.f, 5.0f, 0.1f}, 1.f),//    ngPostRatioSlider.setRange(0.0f, 10.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time",              juce::NormalisableRange<float>{0.f, 3000.0f, 0.1f}, 1.f),//    ngPostAtkSlider.setRange(5.0f, 2000.0f, 5.0f);
				std::make_unique<juce::AudioParameterFloat>("delayFeedback", "Delay Feedback",      juce::NormalisableRange<float>{0.f, 100.0f, 0.1f}, 1.f),//    ngPostRelSlider.setRange(5.0f, 2000.0f, 5.0f);
				std::make_unique<juce::AudioParameterFloat>("delayWet", "Delay Level",              juce::NormalisableRange<float>{0.f, 100.0f, 0.1f}, 1.f),//    lowpassInSlider.setRange(20.0f, 666.0f);
				std::make_unique<juce::AudioParameterFloat>("midEqFreq", "Low-Mid Freq",            juce::NormalisableRange<float>{500.f, 3000.0f, 0.1f}, 1.f),//    gainKnob.setRange(1.0f, 11.0f);
				std::make_unique<juce::AudioParameterFloat>("midEqQ", "Low-Mid Q",                  juce::NormalisableRange<float>{0.f, 1.5f, 0.1f}, 1.f),//    driveSlider.setRange(0.0f, 80.0f);
				std::make_unique<juce::AudioParameterFloat>("midEqGain", "Low-Mid Gain",            juce::NormalisableRange<float>{0.f, 12.f, 0.1f}, 1.f),//    ceilingSlider.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("hiEqFreq", "Hi-Mid Freq",              juce::NormalisableRange<float>{3000.f, 5000.f, 0.1f}, 1.f),//    mixSlider.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("hiEqQ", "Hi-Mid Q",                    juce::NormalisableRange<float>{0.f, 1.5f, 0.1f}, 1.0f),//    threshSlider.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("hiEqGain", "Hi-Mid Gain",              juce::NormalisableRange<float>{0.f, 12.f, 0.1f}, 1.0f),//    reverbWidthSlider.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("reverbRoom", "Reverb Room Size",       juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    reverbDryLevel.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("reverbDamp", "Reverb Dampening",       juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    reverbWetLevel.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("reverbWidth", "Reverb Width",          juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    reverbDampingSlider.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("reverbWet", "Reverb Wet",              juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    tubeMix.setRange(0.0f, 1.0f, .1f);
				std::make_unique<juce::AudioParameterFloat>("reverbDry", "Reverb Dry",              juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    reverbRoomSizeSlider.setRange(0.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("drive", "Distortion Level",            juce::NormalisableRange<float>{0.f, 90.0f, 0.1f}, 1.0f),//    tubeOG.setRange(0.0f, 5.0f);
				std::make_unique<juce::AudioParameterFloat>("driveThresh", "Distortion Press UP",   juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    tubeIG.setRange(0.0f, 2.0f, .1f);
				std::make_unique<juce::AudioParameterFloat>("driveCeil", "Distortion Slam DOWN",    juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    tubeBias.setRange(0.0f, 3.0f);
				std::make_unique<juce::AudioParameterFloat>("driveMix", "Distortion Mix",           juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),//    tubeDrive.setRange(0.0f, 5.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPreThresh", "NG1 Thresh",            juce::NormalisableRange<float>{-60.f, 30.0f, 0.1f}, 1.0f),//    delayTime.setRange(0.0f, 3000.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPreRatio", "NG1 Ratio",              juce::NormalisableRange<float>{0.2f, 10.0f, 0.1f}, 1.0f),//    delayFeedback.setRange(0.0f, 100.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPreAtk", "NG1 Attack",               juce::NormalisableRange<float>{0.5f, 10.0f, 0.1f}, 1.0f),//    hiEqFreqKnob.setRange(3000.0f, 5000.0f, 10.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPreRel", "NG1 Release",              juce::NormalisableRange<float>{1.0f, 1000.0f, 0.1f}, 1.0f),//    delayWet.setRange(0.0f, 100.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPostThresh", "NG2 Thresh",           juce::NormalisableRange<float>{-30.f, 30.0f, 0.1f}, 1.0f),//    hiEqQKnob.setRange(0.01f, 1.5f, 0.01f);
				std::make_unique<juce::AudioParameterFloat>("ngPostRatio", "NG2 Ratio",             juce::NormalisableRange<float>{0.2f, 10.0f, 0.1f}, 1.0f),//    hiEqGainKnob.setRange(-12.0f, 12.0f, 1.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPostAtk", "NG2 Attack",              juce::NormalisableRange<float>{0.5f, 11.0f, 0.1f}, 1.0f),//    midEqFreqKnob.setRange(500.0f, 3000.0, 10.0f);
				std::make_unique<juce::AudioParameterFloat>("ngPostRel", "NG2 Release",             juce::NormalisableRange<float>{1.0f, 1000.0f, 0.1f}, 1.0f),//    midEqQKnob.setRange(0.01f, 1.5f, 0.01f);
				std::make_unique<juce::AudioParameterFloat>("tightenLp", "Tighten Lowend",          juce::NormalisableRange<float>{0.0f, 11.0f, 0.1f}, 20.0f)	//    midEqGainKnob.setRange(-12.0f, 12.0f, 0.5f);
			};
		}
	};
}
