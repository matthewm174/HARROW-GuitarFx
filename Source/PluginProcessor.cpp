/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "ParameterState.h"
#include "PresetManager.h"


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
    , harrowParams(*this, nullptr, ProjectInfo::projectName, Utility::ParameterHelper::createParameterLayout())
{
    harrowParams.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
    harrowParams.state.setProperty("version", ProjectInfo::versionString, nullptr);

    presetManager = std::make_unique<Service::PresetManager>(harrowParams);
    oscillator.initialise([](float x) { return std::sin(x); });
}

DistAdvAudioProcessor::~DistAdvAudioProcessor() {

}


void DistAdvAudioProcessor::updateParameters() {
    //ng1
    auto& ng1Proccessor = processorChain.get<0>();
    processorChain.setBypassed<0>(harrowParams.getRawParameterValue("ng1On")->load());

    ng1Proccessor.setAttack(harrowParams.getRawParameterValue("ngPreAtk")->load());
    ng1Proccessor.setRatio(harrowParams.getRawParameterValue("ngPreRatio")->load());
    ng1Proccessor.setRelease(harrowParams.getRawParameterValue("ngPreRel")->load());
    ng1Proccessor.setThreshold(harrowParams.getRawParameterValue("ngPreThresh")->load());

    //dist
    auto& distortionProcessor = processorChain.get<1>();
    processorChain.setBypassed<1>(harrowParams.getRawParameterValue("distOn")->load());
    auto dt = static_cast<int>(harrowParams.getRawParameterValue("distType")->load());
    setDistType(dt);
    distortionProcessor.setCeiling(harrowParams.getRawParameterValue("driveCeil")->load());
    distortionProcessor.setDrive(harrowParams.getRawParameterValue("drive")->load());
    distortionProcessor.setMix(harrowParams.getRawParameterValue("driveMix")->load());
    distortionProcessor.setThresh(harrowParams.getRawParameterValue("driveThresh")->load());
    distortionProcessor.setGainKnobVal(harrowParams.getRawParameterValue("gain")->load());
    distortionProcessor.setOutput(1.0f);
    distortionProcessor.setFilter(harrowParams.getRawParameterValue("tightenLp")->load());

    //tube

    auto& tubeProcessor = processorChain.get<2>();
    processorChain.setBypassed<2>(harrowParams.getRawParameterValue("tubeOn")->load());
    tubeProcessor.setBias(harrowParams.getRawParameterValue("tubeMix")->load());
    tubeProcessor.setBias(harrowParams.getRawParameterValue("tubeBias")->load());
    tubeProcessor.setBias(harrowParams.getRawParameterValue("tubeDrive")->load());
    tubeProcessor.setBias(harrowParams.getRawParameterValue("tubeIn")->load());
    tubeProcessor.setBias(harrowParams.getRawParameterValue("tubeOut")->load());

    //ng2
    auto& ng2Proccessor = processorChain.get<3>();
    processorChain.setBypassed<3>(harrowParams.getRawParameterValue("ng2On")->load());

    ng2Proccessor.setAttack(harrowParams.getRawParameterValue("ngPostAtk")->load());
    ng2Proccessor.setRatio(harrowParams.getRawParameterValue("ngPostRatio")->load());
    ng2Proccessor.setRelease(harrowParams.getRawParameterValue("ngPostRel")->load());
    ng2Proccessor.setThreshold(harrowParams.getRawParameterValue("ngPostThresh")->load());


    //cab
    auto& cabProcessor = processorChain.get<4>();
    processorChain.setBypassed<4>(harrowParams.getRawParameterValue("cabOn")->load());
    //auto* isSet = /*harrowParams.getRawParameterValue*/("irFile");
    if(harrowParams.state.hasProperty("irFile")) {
        juce::String filepath = harrowParams.state.getProperty("irFile").toString();
        juce::File savedFile(filepath);
        cabProcessor.changeIr(savedFile);
    }

    //EQs
    auto& eq1Processor = processorChain.get<5>();
    //processorChain.setBypassed<5>(harrowParams.getRawParameterValue("eq")->load());

    eq1Processor.setFreq(harrowParams.getRawParameterValue("midEqFreq")->load());
    eq1Processor.setQ(harrowParams.getRawParameterValue("midEqQ")->load());
    eq1Processor.setGain(harrowParams.getRawParameterValue("midEqGain")->load());


    auto& eq2Processor = processorChain.get<6>();
    eq2Processor.setFreq(harrowParams.getRawParameterValue("hiEqFreq")->load());
    eq2Processor.setQ(harrowParams.getRawParameterValue("hiEqQ")->load());
    eq2Processor.setGain(harrowParams.getRawParameterValue("hiEqGain")->load());


    //verb
    auto& verbProcessor = processorChain.get<7>();
    processorChain.setBypassed<7>(harrowParams.getRawParameterValue("reverbOn")->load());

    juce::dsp::Reverb::Parameters params;
    params.roomSize = harrowParams.getRawParameterValue("reverbRoom")->load();
    params.damping = harrowParams.getRawParameterValue("reverbDamp")->load();
    params.width = harrowParams.getRawParameterValue("reverbWidth")->load();
    params.wetLevel = harrowParams.getRawParameterValue("reverbWet")->load();
    params.dryLevel = harrowParams.getRawParameterValue("reverbDry")->load();
    verbProcessor.setParameters(params);
    //delay
    auto& delayProcessor = processorChain.get<8>();
    processorChain.setBypassed<8>(harrowParams.getRawParameterValue("delayOn")->load());
    delayProcessor.setDelayTime(harrowParams.getRawParameterValue("delayTime")->load());
    delayProcessor.setFeedback(harrowParams.getRawParameterValue("delayFeedback")->load());
    delayProcessor.setWetMix(harrowParams.getRawParameterValue("delayWet")->load());
    


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
    tunerOn = false;
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    oscillator.prepare(spec);
    oscillator.setFrequency(440.0f);
    processorChain.get<5>().setType(EqType::PEQ);
    processorChain.get<6>().setType(EqType::PEQ);
    updateParameters();

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

void DistAdvAudioProcessor::processTunerSamples(const float* inputSamples, int numSamples) {

    for (int l = 0; l < numSamples; ++l) {

        if (tunerCount < tunerRecordSize) {

            // Record samples (only use left / first channel)
            tunerRecordedSamples[tunerCount] = inputSamples[l];
            tunerCount++;

        }
        else {

            // After samples are recorded
            tunerSum = 0;
            tunerPdState = 0;
            int period = 0;

            for (int i = 0; i < tunerRecordSize; i++) {

                // Autocorrelation
                tunerSumOld = tunerSum;
                tunerSum = 0;

                for (int k = 0; k < tunerRecordSize - i; k++) {
                    tunerSum += tunerRecordedSamples[k] * tunerRecordedSamples[k + i];
                }

                // Peak Detect State Machine
                if (tunerPdState == 2 && (tunerSum - tunerSumOld) <= 0) {
                    period = i;
                    tunerPdState = 3;
                }

                if (tunerPdState == 1 && (tunerSum > tunerThresh) && (tunerSum - tunerSumOld) > 0) {
                    tunerPdState = 2;
                }

                if (i == 0) {
                    tunerThresh = tunerSum * 0.5;
                    tunerPdState = 1;
                }
            }

            // Frequency identified in Hz
            if (tunerThresh > 10) {
                tunerfrequency = getSampleRate() / period;
            }
            tunerCount = 0;
        }
    }
}



void DistAdvAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);



    //testing sine, uncomment when needed
    //oscillator.process(juce::dsp::ProcessContextReplacing<float>(block));

    if (tunerOn) {
        auto* channelData = buffer.getReadPointer(0); // Assuming we're using the first (left) channel
        int numSamples = buffer.getNumSamples();
        processTunerSamples(channelData, numSamples);
        buffer.clear();
    }
    else {
        
        processorChain.process(context);
        scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());
    }



}

//==============================================================================
bool DistAdvAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistAdvAudioProcessor::createEditor()
{
    return new DistAdvAudioProcessorEditor(*this, harrowParams);

}

//==============================================================================
void DistAdvAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    const auto state = harrowParams.copyState();
    const auto xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DistAdvAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    const auto xmlState = getXmlFromBinary(data, sizeInBytes);
    if (xmlState == nullptr)
        return;
    const auto newTree = juce::ValueTree::fromXml(*xmlState);
    harrowParams.replaceState(newTree);
    //if (harrowParams.state.getProperty("irFile")) {
    //    DBG("Test");
    //}

}

//EQ

void DistAdvAudioProcessor::setHiFreq(float _freq)
{
    processorChain.get<6>().setFreq(_freq);
}
void DistAdvAudioProcessor::setHiQ(float _q)
{
    processorChain.get<6>().setQ(_q);
}
void DistAdvAudioProcessor::setHiGain(float _freq)
{
    processorChain.get<6>().setGain(_freq);
}



void DistAdvAudioProcessor::setMidFreq(float _freq)
{
    processorChain.get<5>().setFreq(_freq);
}

void DistAdvAudioProcessor::setMidQ(float _q)
{
    processorChain.get<5>().setQ(_q);
}
 
void DistAdvAudioProcessor::setMidGain(float _freq)
{
    processorChain.get<5>().setGain(_freq);
}


//dist settings
void DistAdvAudioProcessor::setDist(float gain)
{
    processorChain.get<1>().setDrive(gain);
}

void DistAdvAudioProcessor::setGainKnob(float gain)
{
    processorChain.get<1>().setGainKnobVal(gain);
}

void DistAdvAudioProcessor::setMix(float newMix)
{
    processorChain.get<1>().setMix(newMix);
}

void DistAdvAudioProcessor::setCeiling(float ceil)
{
    processorChain.get<1>().setCeiling(ceil);
}
void DistAdvAudioProcessor::setThresh(float tr)
{
    processorChain.get<1>().setThresh(tr);
}

void DistAdvAudioProcessor::setDistType(int selection)
{
    auto& distortionProcessor = processorChain.get<1>();
    switch (selection)
    {
        case 0: distortionProcessor.setDistModel(Distortion<float>::DistModel::kHard); break;
        case 1: distortionProcessor.setDistModel(Distortion<float>::DistModel::kSoft); break;
        case 2: distortionProcessor.setDistModel(Distortion<float>::DistModel::kSat); break;
        case 3: distortionProcessor.setDistModel(Distortion<float>::DistModel::kHard2); break;
        case 4: distortionProcessor.setDistModel(Distortion<float>::DistModel::kFuzz); break;
        case 5: distortionProcessor.setDistModel(Distortion<float>::DistModel::kDiode); break;
        case 6: distortionProcessor.setDistModel(Distortion<float>::DistModel::kLofi); break;
    }
}

void DistAdvAudioProcessor::setFilterInFreq(float freq) {
    processorChain.get<1>().setFilter(freq);
}


// end dist

//tube
void DistAdvAudioProcessor::setTubeDrive(float tubedr) {
    processorChain.get<2>().setDrive(tubedr);
}

void DistAdvAudioProcessor::setTubeBias(float tubebias) {
    processorChain.get<2>().setBias(tubebias);
}

void DistAdvAudioProcessor::setTubeMix(float tubemix) {
    processorChain.get<2>().setMix(tubemix);
}

void DistAdvAudioProcessor::setTubeInputGain(float tubeig) {
    processorChain.get<2>().setInputGain(tubeig);
}

void DistAdvAudioProcessor::setTubeOutputGain(float tubeog) {
    processorChain.get<2>().setOutputGain(tubeog);
}
//end tube

//verb and cab

void DistAdvAudioProcessor::setCab(juce::File f)
{
    harrowParams.state.setProperty("irFile", f.getFullPathName(), nullptr);
    processorChain.get<4>().reset();
    processorChain.get<4>().changeIr(f);
}


void DistAdvAudioProcessor::setReverbParameters(const juce::dsp::Reverb::Parameters& params)
{
    processorChain.get<7>().setParameters(params);
}

//delay

void DistAdvAudioProcessor::setDelay(float delaytime)
{
    processorChain.get<8>().setDelayTime(delaytime);
}

void DistAdvAudioProcessor::setDelayFb(float fb)
{
    processorChain.get<8>().setFeedback(fb);
}

void DistAdvAudioProcessor::setDelayWet(float wm)
{
    processorChain.get<8>().setWetMix(wm);
}

//noise gate pre
void DistAdvAudioProcessor::setNgPreThresh(float ngpret)
{
    processorChain.get<0>().setThreshold(ngpret);
}

void DistAdvAudioProcessor::setNgPreRatio(float ngprerat)
{
    processorChain.get<0>().setRatio(ngprerat);

}

void DistAdvAudioProcessor::setNgPreAtk(float ngprea)
{
    processorChain.get<0>().setAttack(ngprea);

}

void DistAdvAudioProcessor::setNgPreRel(float ngprer)
{
    processorChain.get<0>().setRelease(ngprer);

}

void DistAdvAudioProcessor::setNgPostThresh(float ngpostt)
{
    processorChain.get<3>().setThreshold(ngpostt);

}

void DistAdvAudioProcessor::setNgPostRatio(float ngpostrat)
{
    processorChain.get<3>().setRatio(ngpostrat);

}

void DistAdvAudioProcessor::setNgPostAtk(float ngposta)
{
    processorChain.get<3>().setAttack(ngposta);

}

void DistAdvAudioProcessor::setNgPostRel(float ngpostr)
{
    processorChain.get<3>().setRelease(ngpostr);

}

//bypassing




void DistAdvAudioProcessor::bypassNgPre(bool a)
{
    //apvts.state.setProperty(parameterID, value, nullptr);
    //harrowParams.state.
    processorChain.setBypassed<0>(a);
}
void DistAdvAudioProcessor::bypassDist(bool a)
{
    processorChain.setBypassed<1>(a);
}
void DistAdvAudioProcessor::bypassTube(bool a)
{
    processorChain.setBypassed<2>(a);
}
void DistAdvAudioProcessor::bypassNgPost(bool a)
{
    processorChain.setBypassed<3>(a);
}

void DistAdvAudioProcessor::bypassCab(bool a)
{
    processorChain.setBypassed<4>(a);
}
//5-6 for EQ.
void DistAdvAudioProcessor::bypassDelay(bool a)
{
    processorChain.setBypassed<8>(a);
}

void DistAdvAudioProcessor::bypassReverb(bool a)
{
    processorChain.setBypassed<7>(a);
}



bool DistAdvAudioProcessor::bypassTuner(bool a)
{
    tunerOn = a;
    return a;
}

//void DistAdvAudioProcessor::toggleState(juce::String param)
//{
//    if (auto* paramp = harrowParams.getParameter(param))
//    {
//        bool currentValue = paramp->getValue() >= 0.5f;
//        bool newValue = !currentValue;
//
//        //paramp->beginChangeGesture();
//        paramp->setValueNotifyingHost(newValue ? 1.0f : 0.0f);
//        //paramp->endChangeGesture();
//    }
//}
//
//bool DistAdvAudioProcessor::getToggleState(juce::String param)
//{
//    if (auto* reverbParam = harrowParams.getParameter(param))
//    {
//        return reverbParam->getValue() >= 0.5f;
//    }
//    return false;
//}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistAdvAudioProcessor();
}
