/*
  ==============================================================================

    Oscilloscope.h
    Created: 3 Aug 2024 4:34:47pm
    Author:  mattm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
template <typename SampleType>
class AudioBufferQueue
{
public:
    static constexpr size_t order = 9;
    static constexpr size_t bufferSize = 1U << order;
    static constexpr size_t numBuffers = 5;

    void push(const SampleType* dataToPush, size_t numSamples)
    {
        jassert(numSamples <= bufferSize);

        int start1, size1, start2, size2;
        abstractFifo.prepareToWrite(1, start1, size1, start2, size2);

        jassert(size1 <= 1);
        jassert(size2 == 0);

        if (size1 > 0)
            juce::FloatVectorOperations::copy(buffers[(size_t)start1].data(), dataToPush, (int)juce::jmin(bufferSize, numSamples));

        abstractFifo.finishedWrite(size1);
    }

    void pop(SampleType* outputBuffer)
    {
        int start1, size1, start2, size2;
        abstractFifo.prepareToRead(1, start1, size1, start2, size2);

        jassert(size1 <= 1);
        jassert(size2 == 0);

        if (size1 > 0)
            juce::FloatVectorOperations::copy(outputBuffer, buffers[(size_t)start1].data(), (int)bufferSize);

        abstractFifo.finishedRead(size1);
    }

private:
    juce::AbstractFifo abstractFifo{ numBuffers };
    std::array<std::array<SampleType, bufferSize>, numBuffers> buffers;
};

template <typename SampleType>
class ScopeDataCollector
{
public:
    ScopeDataCollector(AudioBufferQueue<SampleType>& queueToUse)
        : audioBufferQueue(queueToUse)
    {}
    void process(const SampleType* data, size_t numSamples) {
        size_t index = 0;
        if (state == State::waitingForTrigger) {
            while (index++ < numSamples) {
                auto currentSample = *data++;
                //only draw if current sample > trigger thresh
                if (currentSample >= triggerLevel && prevSample < triggerLevel) {
                    numCollected = 0;
                    state = State::collecting;
                    break;
                }
                prevSample = currentSample;
            }
        }

        if (state == State::collecting) {
            while (index++ < numSamples) {
                buffer[numCollected++] = *data++;

                if (numCollected == buffer.size()) {
                    audioBufferQueue.push(buffer.data(), buffer.size());
                    state = State::waitingForTrigger;
                    prevSample = SampleType(100);
                    break;
                }
            }
        }
    }

private:
    AudioBufferQueue<SampleType>& audioBufferQueue;
    std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer;
    size_t numCollected;
    SampleType prevSample = SampleType(100);

    static constexpr auto triggerLevel = SampleType(0.001);

    enum class State { waitingForTrigger, collecting } state{ State::waitingForTrigger };
};

enum class MeterType
{
    PeakMeter,
    VUMeter
};

template <typename SampleType>
class VUMeter : public juce::Component, private juce::Timer
{
public:
    VUMeter(MeterType type) : meterType(type) {
        //jassert(framesPerSecond > 0 && framesPerSecond < 1000);
        startTimerHz(30); // Update the meter at 30 FPS
    }

    void setLevel(SampleType newLevel) {
        level = juce::jmin(newLevel, SampleType(1.0)); // Ensure the level doesn't exceed 1.0
        repaint();
    }

    void paint(juce::Graphics& g) override {
        auto area = getLocalBounds();
        

        auto bounds = getLocalBounds().toFloat();
        g.setColour(juce::Colours::black);
        g.fillRect(bounds);

        g.setColour(juce::Colours::green);
        auto scaledHeight = bounds.getHeight() * level;
        g.fillRect(bounds.removeFromBottom(scaledHeight));

        drawDbLabels(g, area.toFloat());
    }

private:
    MeterType meterType;
    SampleType level = SampleType(0);

    void timerCallback() override {
        // Smoothly decay the level over time to simulate VU meter behavior
        level *= SampleType(0.97);
        repaint();
    }
    void drawDbLabels(juce::Graphics& g, juce::Rectangle<float> bounds) {
        // Define the dB levels you want to show
        std::vector<float> dBLevels = { 0.0f, -6.0f, -12.0f, -18.0f, -24.0f, -30.0f, -40.0f, -50.0f, -60.0f };

        g.setColour(juce::Colours::red);
        g.setFont(12.0f);

        for (float dB : dBLevels) {
            // Calculate the y-position for each dB level
            float normalizedValue = juce::jmap(dB, -60.0f, 0.0f, 0.0f, 1.0f);
            float yPos = bounds.getHeight() * (1.0f - normalizedValue);

            // Draw the label
            g.drawText(juce::String(dB, 1), bounds.getX(), yPos-70, 150, 150, juce::Justification::centredLeft);
        }
    }

};


template <typename SampleType>
class ScopeComponent : public juce::Component,
    private juce::Timer {
public:
    using Queue = AudioBufferQueue<SampleType>;
    ScopeComponent(Queue& queueToUse) : audioBufferQueue(queueToUse), vuMeter(MeterType::VUMeter), peakMeter(MeterType::PeakMeter){
        sampleData.fill(SampleType(0));
        setFramesPerSecond(30);
        //addAndMakeVisible(vuMeter);
        addAndMakeVisible(peakMeter);
        //vuMeter.setBounds(0,40,50,60);
        
    }
    void setFramesPerSecond(int framesPerSecond)
    {
        jassert(framesPerSecond > 0 && framesPerSecond < 1000);
        startTimerHz(framesPerSecond);

        //addAndMakeVisible(vuMeter);

    }
    void paint(juce::Graphics& g) override
    {

        auto area = getLocalBounds();

        auto h = (SampleType)area.getHeight()/2;
        auto w = (SampleType)area.getWidth();

        auto scopeRect = juce::Rectangle<SampleType>{ SampleType(0), SampleType(0), w, h / 2 };
        plot(sampleData.data(), sampleData.size(), g, scopeRect, SampleType(1), h / 4);

        // Spectrum
        auto spectrumRect = juce::Rectangle<SampleType>{ SampleType(0), h / 2, w, h / 2 };
        plot(spectrumData.data(), spectrumData.size() / 4, g, spectrumRect);
    }

    void resized() override {
        auto area = getLocalBounds();

        auto h = (SampleType)area.getHeight();
        auto w = (SampleType)area.getWidth();
        //getLocalBounds().removeFromRight(30)
        //juce::Rectangle<SampleType>
        peakMeter.setBounds(getLocalBounds().removeFromRight(60).removeFromLeft(30));
        vuMeter.setBounds(getLocalBounds().removeFromRight(30));
        
    }

private:
    VUMeter<SampleType> vuMeter;
    VUMeter<SampleType> peakMeter;
    Queue& audioBufferQueue;
    std::array<SampleType, Queue::bufferSize> sampleData;
    juce::dsp::FFT fft{ Queue::order };
    using WindowFun = juce::dsp::WindowingFunction<SampleType>;
    WindowFun windowFun{ (size_t)fft.getSize(), WindowFun::hann };
    std::array<SampleType, 2 * Queue::bufferSize> spectrumData;

    void timerCallback() override
    {
        audioBufferQueue.pop(sampleData.data());


        SampleType rmsLevel = SampleType(0);
        SampleType peakLevel = SampleType(0);

        juce::FloatVectorOperations::copy(spectrumData.data(), sampleData.data(), (int)sampleData.size());
        auto fftSize = (size_t)fft.getSize();

        jassert(spectrumData.size() == 2 * fftSize);
        windowFun.multiplyWithWindowingTable(spectrumData.data(), fftSize);
        fft.performFrequencyOnlyForwardTransform(spectrumData.data());

        static constexpr auto min_db = SampleType(-60);
        static constexpr auto max_db = SampleType(0);

        for (auto& s : spectrumData) {
            
            s = juce::jmap(juce::jlimit(min_db, max_db, juce::Decibels::gainToDecibels(s) - juce::Decibels::gainToDecibels(SampleType(fftSize))), min_db, max_db, SampleType(0), SampleType(1));
            rmsLevel += s * s;
            peakLevel = juce::jmax(peakLevel, std::abs(s));
            
        }
        rmsLevel = std::sqrt(rmsLevel / (SampleType)sampleData.size());

        auto peakDb = juce::Decibels::gainToDecibels(peakLevel);

        auto mappedLevel = juce::jmap(peakDb, SampleType(-60.0), SampleType(0.0), SampleType(0.0), SampleType(1.0));

        vuMeter.setLevel(rmsLevel);
        peakMeter.setLevel(mappedLevel);//*/; // Update the VU meter with the RMS level
        repaint();
    }
    static void plot(const SampleType* data,
        size_t numSamples,
        juce::Graphics& g,
        juce::Rectangle<SampleType> rect,
        SampleType scaler = SampleType(1),
        SampleType offset = SampleType(0))
    {
        auto w = rect.getWidth();
        auto h = rect.getHeight();
        auto right = rect.getRight();
        auto center = rect.getBottom() - offset;
        auto gain = h * scaler;

        for (size_t i = 1; i < numSamples; ++i)
            g.drawLine({ juce::jmap(SampleType(i - 1), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), SampleType(right)),
                          center - gain * data[i - 1],
                          juce::jmap(SampleType(i), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), SampleType(right)),
                          center - gain * data[i] });
    }
};
