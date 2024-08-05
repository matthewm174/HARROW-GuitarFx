/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters/Parameters.h"

//==============================================================================
DistAdvAudioProcessor::DistAdvAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, _treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    _treeState.addParameterListener(distModelID, this);
    _treeState.addParameterListener(inputID, this);
    _treeState.addParameterListener(mixID, this);
    _treeState.addParameterListener(outputID, this);

}

DistAdvAudioProcessor::~DistAdvAudioProcessor()
{
    _treeState.removeParameterListener(distModelID, this);
    _treeState.removeParameterListener(inputID, this);
    _treeState.removeParameterListener(mixID, this);
    _treeState.removeParameterListener(outputID, this);

}

juce::AudioProcessorValueTreeState::ParameterLayout DistAdvAudioProcessor::createParameterLayout() {
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    juce::StringArray distModels = { "Hard", "Soft", "Saturation" };

    auto pDriveModel = std::make_unique<juce::AudioParameterChoice>(distModelID, distModelName, distModels, 0);
    auto pDrive = std::make_unique<juce::AudioParameterFloat>(inputID, inputName, 0.0f, 24.0f, 0.0f);
    auto pMix = std::make_unique<juce::AudioParameterFloat>(mixID, mixName, 0.0f, 1.0f, 1.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -24.0f, 24.0f, 0.0f);

    params.push_back(std::move(pDriveModel));
    params.push_back(std::move(pDrive));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pOutput));

    return { params.begin(), params.end() };
}

void DistAdvAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    updateParameters();

}

void DistAdvAudioProcessor::updateParameters() {

    auto model = static_cast<int>(_treeState.getRawParameterValue(distModelID)->load());
    switch(model) 
    {
        case 0: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kHard); break;
        case 1: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kSoft); break;
        case 2: processorChain.get<0>().setDistModel(Distortion<float>::DistModel::kSat); break;
    }
    processorChain.get<0>().setDrive(_treeState.getRawParameterValue(inputID)->load());
    processorChain.get<0>().setMix(_treeState.getRawParameterValue(mixID)->load());
    processorChain.get<0>().setOutput(_treeState.getRawParameterValue(outputID)->load());

}

//==============================================================================
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistAdvAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistAdvAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistAdvAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistAdvAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistAdvAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    /*_distortionModule*/
    processorChain.prepare(spec);

    updateParameters();
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistAdvAudioProcessor::releaseResources()
{
    processorChain.reset();
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

juce::AudioProcessorValueTreeState& DistAdvAudioProcessor::getAudioProcessorValueTreeState() {
    return _treeState;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistAdvAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void DistAdvAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    //juce::dsp::AudioBlock<float> block { buffer };
    ////_distortionModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    //for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    //{
    //    auto* channelData = block.getChannelPointer(channel);
    //    for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
    //    {
    //        channelData[sample] = fxChain.get<0>().processSaturation(channelData[sample]);
    //    }
    //}


    //fxChain.get<0>().processSaturation()/*;*/

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
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
void DistAdvAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistAdvAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void DistAdvAudioProcessor::setDist(float gain)
{
    processorChain.get<0>().setDrive(gain);
}
void DistAdvAudioProcessor::setReverbParameters(const juce::dsp::Reverb::Parameters& params)
{
    processorChain.get<2>().setParameters(params);
}
//void Distortion::setDistModel(DistModel model)
//{
//    processorChain.get<0>().setDistModel(model);
//}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistAdvAudioProcessor();
}
