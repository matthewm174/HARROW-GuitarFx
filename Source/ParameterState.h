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
				std::make_unique<juce::AudioParameterFloat>("gain", "Gain",                         juce::NormalisableRange<float>{0.f, 80.0f, 0.1f}, 1.f),     
				std::make_unique<juce::AudioParameterFloat>("tubeMix", "Tube Mix",                  juce::NormalisableRange<float>{0.f, 1.0f, 0.01f}, 1.f),		
				std::make_unique<juce::AudioParameterFloat>("tubeBias", "Tube Bias",                juce::NormalisableRange<float>{0.f, 2.0f, 0.1f}, 1.f),		
				std::make_unique<juce::AudioParameterFloat>("tubeIn", "Tube Input",                 juce::NormalisableRange<float>{0.f, 2.0f, 0.1f}, 1.f),		
				std::make_unique<juce::AudioParameterFloat>("tubeOut", "Tube Output",               juce::NormalisableRange<float>{0.f, 3.0f, 0.1f}, 1.f),		
				std::make_unique<juce::AudioParameterFloat>("tubeDrive", "Tube Drive",              juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.f),		
				std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time",              juce::NormalisableRange<float>{0.f, 3000.0f, 0.1f}, 1.f),	
				std::make_unique<juce::AudioParameterFloat>("delayFeedback", "Delay Feedback",      juce::NormalisableRange<float>{0.f, 100.0f, 0.1f}, 1.f),	
				std::make_unique<juce::AudioParameterFloat>("delayWet", "Delay Level",              juce::NormalisableRange<float>{0.f, 100.0f, 0.1f}, 1.f),	
				std::make_unique<juce::AudioParameterFloat>("midEqFreq", "Low-Mid Freq",            juce::NormalisableRange<float>{500.f, 3000.0f, 0.1f}, 1.f),	
				std::make_unique<juce::AudioParameterFloat>("midEqQ", "Low-Mid Q",                  juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.f),		
				std::make_unique<juce::AudioParameterFloat>("midEqGain", "Low-Mid Gain",            juce::NormalisableRange<float>{-12.f, 12.f, 0.1f}, 1.f),	
				std::make_unique<juce::AudioParameterFloat>("hiEqFreq", "Hi-Mid Freq",              juce::NormalisableRange<float>{3000.f, 5000.f, 0.1f}, 1.f),	
				std::make_unique<juce::AudioParameterFloat>("hiEqQ", "Hi-Mid Q",                    juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("hiEqGain", "Hi-Mid Gain",              juce::NormalisableRange<float>{0.f, 12.f, 0.1f}, 1.0f),		
				std::make_unique<juce::AudioParameterFloat>("reverbRoom", "Reverb Room Size",       juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 0.1f),		
				std::make_unique<juce::AudioParameterFloat>("reverbDamp", "Reverb Dampening",       juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 0.1f),		
				std::make_unique<juce::AudioParameterFloat>("reverbWidth", "Reverb Width",          juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 0.1f),		
				std::make_unique<juce::AudioParameterFloat>("reverbWet", "Reverb Wet",              juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 0.8f),		
				std::make_unique<juce::AudioParameterFloat>("reverbDry", "Reverb Dry",              juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 0.2f),		
				std::make_unique<juce::AudioParameterFloat>("drive", "Distortion Level",            juce::NormalisableRange<float>{0.f, 10.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("driveThresh", "Distortion Press UP",   juce::NormalisableRange<float>{1.f, 0.0f, 0.1f}, 1.0f),		
				std::make_unique<juce::AudioParameterFloat>("driveCeil", "Distortion Slam DOWN",    juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),		
				std::make_unique<juce::AudioParameterFloat>("driveMix", "Distortion Mix",           juce::NormalisableRange<float>{0.f, 1.0f, 0.1f}, 1.0f),		
				std::make_unique<juce::AudioParameterFloat>("ngPreThresh", "NG1 Thresh",            juce::NormalisableRange<float>{-100.f, 30.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("ngPreRatio", "NG1 Ratio",              juce::NormalisableRange<float>{1.5f, 10.0f, 1.5f}, 0.5f),	
				std::make_unique<juce::AudioParameterFloat>("ngPreAtk", "NG1 Attack",               juce::NormalisableRange<float>{0.5f, 10.0f, 0.1f}, 0.5f),	
				std::make_unique<juce::AudioParameterFloat>("ngPreRel", "NG1 Release",              juce::NormalisableRange<float>{1.0f, 1000.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("ngPostThresh", "NG2 Thresh",           juce::NormalisableRange<float>{-30.f, 30.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("ngPostRatio", "NG2 Ratio",             juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("ngPostAtk", "NG2 Attack",              juce::NormalisableRange<float>{0.5f, 11.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("ngPostRel", "NG2 Release",             juce::NormalisableRange<float>{1.0f, 1000.0f, 0.1f}, 1.0f),	
				std::make_unique<juce::AudioParameterFloat>("tightenLp", "Tighten Lowend",          juce::NormalisableRange<float>{10.0f, 666.0f, 1.0f}, 1.0f),	

				std::make_unique<juce::AudioParameterBool>("distOn", "Distortion On", false),
				std::make_unique<juce::AudioParameterBool>("cabOn", "Cabinet On", false),
				std::make_unique<juce::AudioParameterBool>("reverbOn", "Verb On", false),
				std::make_unique<juce::AudioParameterBool>("delayOn", "Delay On", false),
				std::make_unique<juce::AudioParameterBool>("tubeOn", "Tube On", false),
				std::make_unique<juce::AudioParameterBool>("tunerOn", "Tuner On", false),
				std::make_unique<juce::AudioParameterBool>("ng1On", "NG1 On", false),
				std::make_unique<juce::AudioParameterBool>("ng2On", "NG2 On", false),
				std::make_unique<juce::AudioParameterChoice>("distType", "Distortion Type", juce::StringArray{"EGGIE", "DANDY", "SWEETPEA", "DAISY", "SPENCER1", "SPENCER2", "SPENCER3"}, 0),
				//std::make_unique<juce::String>("a"),
				//std::make_unique<juce::AudioParameterChoice>("reverbMode", "Reverb Mode",
						//juce::StringArray{"Small Room", "Large Hall", "Plate"}
			};
		}
	};
}
