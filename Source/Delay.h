/*
  ==============================================================================

    Delay.h
    Created: 11 Aug 2024 9:57:32am
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DelayLine.h"
template <typename SampleType>
class DelayEffect
{
public:
	DelayEffect();
	~DelayEffect();
	void prepare(juce::dsp::ProcessSpec spec);
	void process(juce::dsp::ProcessContextReplacing<SampleType>& context);
	void reset();
	void setDelayTime(float delaytime);
	void setFeedback(float feedbacklvl);
	void setWetMix(float wetmix);

private:
	void update();

	float linearInterpolation(const float& y0, const float& yp1, const float& frac);


	std::array<DelayLine	<float>, 2> mDelayLines;
	std::array<juce::SmoothedValue<float>, 2> mSmoothG;
	std::array<juce::SmoothedValue<float>, 2> mSmoothW;
	std::array<juce::SmoothedValue<float>, 2> mSmoothFB;

	float _feedbackLevel = 1.0f;
	float _time = 1.0f;
	float _wetLevel = 1.0f;


	float mLastTime = 0.f;
	double mSampleRate = 44100.f;
	int mSamplesPerBlock = 512;
	int mDelayBufferLen = 0;
	int mNumChannels = 2;




};
