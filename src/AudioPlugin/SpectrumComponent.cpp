#include "SpectrumComponent.h"
#include "PluginProcessor.h"

SpectrumComponent::SpectrumComponent(AudioPluginAudioProcessor &p)
    : forwardFFT(std::make_unique<juce::dsp::FFT>(fftOrder)),
      window(std::make_unique<juce::dsp::WindowingFunction<float>>(
          fftSize, juce::dsp::WindowingFunction<float>::hann)) {
  setOpaque(true);
  startTimerHz(60);
}

void SpectrumComponent::processAudioBlock(
    const juce::AudioBuffer<float> &buffer) {
  if (buffer.getNumChannels() > 0) {
    auto *channelData = buffer.getReadPointer(0, 0);
    auto numSamples = buffer.getNumSamples();

    for (auto i = 0; i < numSamples; ++i)
      pushNextSampleIntoFifo(channelData[i]);
  }
}

void SpectrumComponent::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::black);

  g.setOpacity(1.0f);
  g.setColour(juce::Colours::white);
  drawFrame(g);
}

void SpectrumComponent::timerCallback() {
  if (nextFFTBlockReady) {
    drawNextFrameOfSpectrum();
    nextFFTBlockReady = false;
    repaint();
  }
}

void SpectrumComponent::pushNextSampleIntoFifo(float sample) noexcept {
  // If the fifo contains enough data, set a flag to say
  // that the next frame should now be rendered
  if (fifoIndex == fftSize) {
    if (!nextFFTBlockReady) {
      juce::zeromem(fftData, sizeof(fftData));
      memcpy(fftData, fifo, sizeof(fifo));
      nextFFTBlockReady = true;
    }
    fifoIndex = 0;
  }
  fifo[fifoIndex++] = sample;
}

void SpectrumComponent::drawNextFrameOfSpectrum() {
  // Apply a windowing function to our data
  window->multiplyWithWindowingTable(fftData, fftSize);

  // Render our FFT data
  forwardFFT->performFrequencyOnlyForwardTransform(fftData);

  auto mindB = -100.0f;
  auto maxdB = 0.0f;

  for (int i = 0; i < scopeSize; ++i) {
    auto skewedProportionX =
        1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
    auto fftDataIndex = juce::jlimit(
        0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
    auto level = juce::jmap(
        juce::jlimit(mindB, maxdB,
                     juce::Decibels::gainToDecibels(fftData[fftDataIndex]) -
                         juce::Decibels::gainToDecibels((float)fftSize)),
        mindB, maxdB, 0.0f, 1.0f);

    scopeData[i] = level;
  }
}

void SpectrumComponent::drawFrame(juce::Graphics &g) {
  for (int i = 1; i < scopeSize; ++i) {
    auto width = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();

    g.drawLine({(float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                (float)juce::jmap(i, 0, scopeSize - 1, 0, width),
                juce::jmap(scopeData[i], 0.0f, 1.0f, (float)height, 0.0f)});
  }
}
