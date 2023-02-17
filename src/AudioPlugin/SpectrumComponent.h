#pragma once

#include <JuceHeader.h>

class AudioPluginAudioProcessor;

// Adapted from: https://docs.juce.com/master/tutorial_spectrum_analyser.html
class JUCE_API SpectrumComponent : public juce::Component, private juce::Timer {
public:
  SpectrumComponent(AudioPluginAudioProcessor &p);

  void processAudioBlock(const juce::AudioBuffer<float> &buffer);
  void paint(juce::Graphics &g) override;
  void timerCallback() override;
  void pushNextSampleIntoFifo(float sample) noexcept;
  void drawNextFrameOfSpectrum();
  void drawFrame(juce::Graphics &g);

  enum { fftOrder = 11, fftSize = 1 << fftOrder, scopeSize = 512 };

private:
  std::unique_ptr<juce::dsp::FFT> forwardFFT;
  std::unique_ptr<juce::dsp::WindowingFunction<float>> window;

  float fifo[fftSize];
  float fftData[2 * fftSize];
  int fifoIndex = 0;
  bool nextFFTBlockReady = false;
  float scopeData[scopeSize];

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumComponent)
};
