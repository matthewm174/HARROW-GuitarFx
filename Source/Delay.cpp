/*
  ==============================================================================

    Delay.cpp
    Created: 11 Aug 2024 9:57:32am
    Author:  mattm

  ==============================================================================
*/

#include "Delay.h"

template <typename SampleType>
DelayEffect<SampleType>::DelayEffect()
{
}


template <typename SampleType>
DelayEffect<SampleType>::~DelayEffect()
{
}


template <typename SampleType>
void DelayEffect<SampleType>::reset()
{
}
template<typename SampleType>
void DelayEffect<SampleType>::setDelayTime(float delaytime)
{
	_time = delaytime;
}
template<typename SampleType>
void DelayEffect<SampleType>::setFeedback(float feedbacklvl)
{
	_feedbackLevel = feedbacklvl;
}
template<typename SampleType>
void DelayEffect<SampleType>::setWetMix(float wetmix)
{
	_wetLevel = wetmix;
}
//==============================================================================

template <typename SampleType>
void DelayEffect<SampleType>::prepare(juce::dsp::ProcessSpec spec)
{

	mSampleRate = spec.sampleRate;
	mSamplesPerBlock = spec.maximumBlockSize;
	mNumChannels = spec.numChannels;
	mDelayBufferLen = 2 * (mSampleRate + mSamplesPerBlock);

	for (auto& dline : mDelayLines)
	{
		dline.resize(mDelayBufferLen);
		dline.clear();
	}

	float FB = _feedbackLevel / 100.f;
	float W = _wetLevel / 100.f;
	float G = 1 - W;
	mLastTime = _time;

	for (int channel = 0; channel < 2; ++channel)
	{
		mSmoothFB[channel].reset(mSamplesPerBlock);
		mSmoothW[channel].reset(mSamplesPerBlock);
		mSmoothG[channel].reset(mSamplesPerBlock);

		mSmoothFB[channel].setCurrentAndTargetValue(FB);
		mSmoothW[channel].setCurrentAndTargetValue(W);
		mSmoothG[channel].setCurrentAndTargetValue(G);
	}
}

template <typename SampleType>
void DelayEffect<SampleType>::process(juce::dsp::ProcessContextReplacing<SampleType>& context)
{
	const auto& inputBlock = context.getInputBlock();
	auto& outputBlock = context.getOutputBlock();
	const auto numChannels = outputBlock.getNumChannels();
	const auto numSamples = outputBlock.getNumSamples();

	if (context.isBypassed)
	{
		outputBlock.copyFrom(inputBlock);
		return;
	}

	float time = _time;
	update();

	float delayInSamplesFrac = mSampleRate * (time / 1000.f);
	int delayInSamplesInt = static_cast<int> (delayInSamplesFrac);
	float frac = delayInSamplesFrac - delayInSamplesInt;

	for (size_t channel = 0; channel < numChannels; ++channel)
	{

		float FB = mSmoothFB[channel].getNextValue();
		float W = mSmoothW[channel].getNextValue();
		float G = mSmoothG[channel].getNextValue();

		auto* channelData = outputBlock.getChannelPointer(static_cast<int>(channel));


		for (size_t sample = 0; sample < numSamples; ++sample)
		{
			float delayedSample = 0.f;

			if (frac != 0)
			{
				float y0 = mDelayLines[channel].get(delayInSamplesInt);
				float ym1 = mDelayLines[channel].get(delayInSamplesInt + 1);
				delayedSample = linearInterpolation(y0, ym1, frac);
			}
			else
			{
				delayedSample = mDelayLines[channel].get(delayInSamplesInt);
			}

			auto inputSample = channelData[sample];
			mDelayLines[channel].push(inputSample + delayedSample * FB);
			channelData[sample] = inputSample * G + delayedSample * W;
		}
	}
}

template <typename SampleType>
void DelayEffect<SampleType>::update()
{

	float FB = _feedbackLevel / 100.f;
	float W = _wetLevel / 100.f;
	float G = 1 - W;

	float time = _time;

	if (time != mLastTime)
	{
		W = 0;
		G = 1;
		for (auto& dline : mDelayLines)
		{
			//clear to reduce crackling
			dline.clear();
		}
	}
	mLastTime = time;

	for (int channel = 0; channel < 2; ++channel)
	{
		mSmoothFB[channel].setValue(FB);
		mSmoothW[channel].setValue(W);
		mSmoothG[channel].setValue(G);
	}
}


template <typename SampleType>
float DelayEffect<SampleType>::linearInterpolation(const float& y0, const float& yp1, const float& frac)
{
	return yp1 * frac + y0 * (1 - frac);
}

template class DelayEffect<float>;
template class DelayEffect<double>;