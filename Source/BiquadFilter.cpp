/*
  ==============================================================================

    BiquadFilter.cpp
    Created: 18 Aug 2024 9:59:20pm
    Author:  mattm

  ==============================================================================
*/

#include "BiquadFilter.h"
BiquadFilter::BiquadFilter() {
}

BiquadFilter::~BiquadFilter() {
}

void BiquadFilter::setFilterCoefficients(float _a1, float _a2, float _b0, float _b1, float _b2) {
	a1 = _a1;
	a2 = _a2;
	b0 = _b0;
	b1 = _b1;
	b2 = _b2;
}

float BiquadFilter::doFilter(float _input) {
    jassert(sampleRate > 0);


	//https://www.youtube.com/watch?v=3ZHcmeTvhe4
	float adder1 = 
	_input 
	- z2 * a2 
	- z1 * a1;
	float adder2 = 
	  b2 * z2 
	+ b1 * z1 
	+ b0 * adder1;

	z2 = z1;
	z1 = adder1;

	return adder2;
}