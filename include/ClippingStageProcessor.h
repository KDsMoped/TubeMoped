#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "BiquadFilterDF2.h"

namespace TubeMoped
{
    class ClippingStageProcessor
    {
    public:
        ClippingStageProcessor(float samplerate)
        : samplerate_(samplerate)
        {
            for(auto c = size_t(0); c < 2; ++c)
            {
                highpassFilters_.push_back(std::make_unique<kdsp::BiquadFilterDF2<float>>(2));
                lowpassFilters_.push_back(std::make_unique<kdsp::BiquadFilterDF2<float>>(2));
            }
        }

        void process(juce::AudioBuffer<float>& buffer, float distortion)
        {
            auto distortionRes = 500000.f * distortion;

            for(auto& hpf : highpassFilters_)
            {
                hpf->calcFilterCoeffs(kdsp::ZoelzerHighpass, samplerate_, 720.f, 0.707f);
            }

            auto f = 1.f / (juce::MathConstants<float>::twoPi * (51000.f + distortionRes) * 51.f * std::powf(10.f, -12.f));
            for(auto& lpf : lowpassFilters_)
            {
                lpf->calcFilterCoeffs(kdsp::ZoelzerLowpass, samplerate_, f, 0.707f);
            }

            auto opampGain = 1 + ((51000.f + distortionRes) / 4700.f);

            buffer.applyGain(juce::Decibels::decibelsToGain<float>(opampGain));

            clipSignal(buffer);

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
                    float hpFiltered = highpassFilters_[c]->process(clipped);
                    float lpFiltered = lowpassFilters_[c]->process(hpFiltered);
                    channelBuffer[s] = lpFiltered;
                }
            }
        }

        float samplerate_;

        std::vector<std::unique_ptr<kdsp::BiquadFilterDF2<float>>> highpassFilters_;
        std::vector<std::unique_ptr<kdsp::BiquadFilterDF2<float>>> lowpassFilters_;

    };
}
