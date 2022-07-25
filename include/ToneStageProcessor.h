#pragma once 

namespace TubeMoped
{
    class ToneStageProcessor
    {
    public:
        ToneStageProcessor(float samplerate, int blocksize, int channels)
        : samplerate_(samplerate)
        , lpBuffer_(channels, blocksize)
        , hpBuffer_(channels, blocksize)
        {
            passiveLowpass_.prepare(juce::dsp::ProcessSpec{samplerate, static_cast<unsigned int>(blocksize), static_cast<unsigned int>(channels)});
            passiveLowpass_.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
            passiveLowpass_.setCutoffFrequency(723.4f);
            
            activeLowpass_.prepare(juce::dsp::ProcessSpec{samplerate, static_cast<unsigned int>(blocksize), static_cast<unsigned int>(channels)});
            activeLowpass_.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
            //const auto f = 1.f / (juce::MathConstants<float>::twoPi * (220.f) * std::powf(0.22f, -6.f)); Correct calculation resulting near 0?
            activeLowpass_.setCutoffFrequency(1000.f);
            
            activeHighpass_.prepare(juce::dsp::ProcessSpec{samplerate, static_cast<unsigned int>(blocksize), static_cast<unsigned int>(channels)});
            activeHighpass_.setType(juce::dsp::FirstOrderTPTFilterType::highpass);
            activeHighpass_.setCutoffFrequency(1000.f);
        }

        void process(juce::AudioBuffer<float>& buffer, float tone)
        {
        
            auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
            auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
            passiveLowpass_.process(context);

            lpBuffer_ = buffer;
            hpBuffer_ = buffer;

            auto lpAudioBlock = juce::dsp::AudioBlock<float>(lpBuffer_);
            auto lpContext = juce::dsp::ProcessContextReplacing<float>(lpAudioBlock);
            activeLowpass_.process(lpContext);
            
            auto hpAudioBlock = juce::dsp::AudioBlock<float>(hpBuffer_);
            auto hpContext = juce::dsp::ProcessContextReplacing<float>(hpAudioBlock);
            activeHighpass_.process(hpContext);

            for(auto c = size_t(0); c < buffer.getNumChannels(); ++c)
            {
                auto channelBuffer = buffer.getWritePointer(static_cast<int>(c));
                const auto lpChannelBuffer = lpBuffer_.getReadPointer(static_cast<int>(c));
                const auto hpChannelBuffer = hpBuffer_.getReadPointer(static_cast<int>(c));

                for(auto s = size_t(0); s < buffer.getNumSamples(); ++s)
                {
                    channelBuffer[s] = ((1.f - tone) * lpChannelBuffer[s]) + (tone * hpChannelBuffer[s]);
                }
            }
        }

    private:
        float samplerate_;

        juce::AudioBuffer<float> lpBuffer_;
        juce::AudioBuffer<float> hpBuffer_;

        juce::dsp::FirstOrderTPTFilter<float> passiveLowpass_;

        juce::dsp::FirstOrderTPTFilter<float> activeLowpass_;
        juce::dsp::FirstOrderTPTFilter<float> activeHighpass_;
    };
}
