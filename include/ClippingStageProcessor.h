#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace TubeMoped
{
    namespace
    {
        void clipSignal(juce::AudioBuffer<float>& buffer)
        {
            for(auto c = size_t(0); c < buffer.getNumChannels(); ++c)
            {
                auto channelBuffer = buffer.getWritePointer(c);
                for(auto s = size_t(0); s < buffer.getNumSamples(); ++s)
                {
                    float sample = channelBuffer[s];
                    channelBuffer[s] = juce::jmax<float>(juce::jmin<float>(sample, 1.f), -1);
                }
            }
        }
    }

    class ClippingStageProcessor
    {
    public:
        void process(juce::AudioBuffer<float>& buffer)
        {
            auto distortionRes = 250;

            auto opampGain = 1 + (51 + distortionRes) / 4.7f;

            buffer.applyGain(juce::Decibels::decibelsToGain<float>(opampGain));

            clipSignal(buffer);
        }
    };
}
