/*
  ==============================================================================

    BiquadFilter.h
    Created: 18 Aug 2024 9:59:20pm
    Author:  mattm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class BiquadFilter {
public:
  BiquadFilter();
  virtual ~BiquadFilter();

  inline void reset() { z1 = z2 = 0.0f; }

  void setFilterCoefficients(float _a1, float _a2, float _b0, float _b1,
                             float _b2);

  virtual float doFilter(float _input);

  virtual void setSampleRate(float _sampleRate){ 
		sampleRate = _sampleRate;
		oneOverSamplerate = 1.0f / _sampleRate;
	}

protected:
  float a1 = 1.0f;
  float a2 = 1.0f;
  float b0 = 1.0f;
  float b1 = 1.0f;
  float b2 = 1.0f;

  float z1 = 0.0f;
  float z2 = 0.0f;

  float sampleRate = -1;
  float oneOverSamplerate;
};