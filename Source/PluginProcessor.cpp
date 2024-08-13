/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"


DistAdvAudioProcessor::DistAdvAudioProcessor() 
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::mono(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    oscillator.initialise([](float x) { return std::sin(x); });
}

DistAdvAudioProcessor::~DistAdvAudioProcessor() {

}


void DistAdvAudioProcessor::updateParameters(int selection) {
    switch (selection)
    {
    case 1: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kHard); break;
    case 2: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kSoft); break;
    case 3: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kSat); break;
    case 4: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kHard2); break;
    }
}



const juce::String DistAdvAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistAdvAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DistAdvAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DistAdvAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DistAdvAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistAdvAudioProcessor::getNumPrograms()
{
    return 1;   
}

int DistAdvAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistAdvAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String DistAdvAudioProcessor::getProgramName(int index)
{
    return {};
}

void DistAdvAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void DistAdvAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    oscillator.prepare(spec);

    oscillator.setFrequency(100.0f);


    processorChain.prepare(spec);

}

void DistAdvAudioProcessor::releaseResources()
{
    processorChain.reset();

}


#ifndef JucePlugin_PreferredChannelConfigurations
bool DistAdvAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void DistAdvAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    //testing sine, uncomment when needed
    //oscillator.process(juce::dsp::ProcessContextReplacing<float>(block));


    processorChain.process(context);

    scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());

}

//==============================================================================
bool DistAdvAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistAdvAudioProcessor::createEditor()
{
    return new DistAdvAudioProcessorEditor(*this);

}

//==============================================================================
void DistAdvAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistAdvAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void DistAdvAudioProcessor::setDist(float gain)
{
    processorChain.get<0>().setDrive(gain);
}

void DistAdvAudioProcessor::setGainKnob(float gain)
{
    processorChain.get<0>().setGainKnobVal(gain);
}

void DistAdvAudioProcessor::setMix(float newMix)
{
    processorChain.get<0>().setMix(newMix);
}

void DistAdvAudioProcessor::setCeiling(float ceil)
{
    processorChain.get<0>().setCeiling(ceil);
}
void DistAdvAudioProcessor::setThresh(float tr)
{
    processorChain.get<0>().setThresh(tr);
}

void DistAdvAudioProcessor::setDelay(float delaytime)
{
    processorChain.get<4>().setDelayTime(delaytime);
}

void DistAdvAudioProcessor::setDelayFb(float fb)
{
    processorChain.get<4>().setFeedback(fb);
}

void DistAdvAudioProcessor::setDelayWet(float wm)
{
    processorChain.get<4>().setWetMix(wm);
}

void DistAdvAudioProcessor::setReverbParameters(const juce::dsp::Reverb::Parameters& params)
{
    processorChain.get<3>().setParameters(params);
}

void DistAdvAudioProcessor::bypassDelay()
{
    auto x = processorChain.isBypassed<4>();
    processorChain.setBypassed<4>(!x);
}

void DistAdvAudioProcessor::bypassReverb()
{
    auto x = processorChain.isBypassed<3>();
    processorChain.setBypassed<3>(!x);
}

void DistAdvAudioProcessor::bypassCab()
{
    auto x = processorChain.isBypassed<2>();
    processorChain.setBypassed<2>(!x);
}

void DistAdvAudioProcessor::bypassDist()
{
    auto x = processorChain.isBypassed<0>();
    processorChain.setBypassed<0>(!x);
}

void DistAdvAudioProcessor::bypassTube()
{
    auto x = processorChain.isBypassed<1>();
    processorChain.setBypassed<1>(!x);
}

void DistAdvAudioProcessor::setCab(juce::File f)
{
    processorChain.get<2>().reset();
    return processorChain.get<2>().changeIr(f);
}

void DistAdvAudioProcessor::setTubeDrive(float tubedr) {
    processorChain.get<1>().setDrive(tubedr);
}

void DistAdvAudioProcessor::setTubeBias(float tubedr) {
    processorChain.get<1>().setBias(tubedr);
}

void DistAdvAudioProcessor::setTubeMix(float tubedr) {
    processorChain.get<1>().setMix(tubedr);
}

void DistAdvAudioProcessor::setTubeInputGain(float tubedr) {
    processorChain.get<1>().setInputGain(tubedr);
}

void DistAdvAudioProcessor::setTubeOutputGain(float tubedr) {
    processorChain.get<1>().setOutputGain(tubedr);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistAdvAudioProcessor();
}
