#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "BiquadFilterDF2.h"

namespace TubeMoped
{
    class ClippingStageProcessor
    {
    public:
        ClippingStageProcessor(float samplerate, int blocksize, int channels)
        : samplerate_(samplerate)
        {
            highpass_.prepare(juce::dsp::ProcessSpec{samplerate_, static_cast<unsigned int>(blocksize), static_cast<unsigned int>(channels)});
            highpass_.setType(juce::dsp::FirstOrderTPTFilterType::highpass);
            lowpass_.prepare(juce::dsp::ProcessSpec{samplerate_, static_cast<unsigned int>(blocksize), static_cast<unsigned int>(channels)});
            lowpass_.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
        }

        void process(juce::AudioBuffer<float>& buffer, float distortion)
        {
            auto distortionRes = 500000.f * distortion;

            highpass_.setCutoffFrequency(720.f);

            auto f = 1.f / (juce::MathConstants<float>::twoPi * (51000.f + distortionRes) * 51.f * std::powf(10.f, -12.f));
            lowpass_.setCutoffFrequency(juce::jmin(f, 0.5f * samplerate_));

            auto opampGain = 1 + ((51000.f + distortionRes) / 4700.f);

            buffer.applyGain(juce::Decibels::decibelsToGain<float>(opampGain));

            clipSignal(buffer);

            auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
            auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
            highpass_.process(context);
            lowpass_.process(context);
        }

    private:
        void clipSignal(juce::AudioBuffer<float>& buffer)
        {
            for(auto c = size_t(0); c < buffer.getNumChannels(); ++c)
            {
                auto channelBuffer = buffer.getWritePointer(static_cast<int>(c));
                for(auto s = size_t(0); s < buffer.getNumSamples(); ++s)
                {
                    float sample = channelBuffer[s];
                    float clipped = juce::jmax<float>(juce::jmin<float>(sample, 1.f), -1);
                    channelBuffer[s] = clipped;
                }
            }
        }

        float samplerate_;

        juce::dsp::FirstOrderTPTFilter<float> highpass_;
        juce::dsp::FirstOrderTPTFilter<float> lowpass_;

    };
}
