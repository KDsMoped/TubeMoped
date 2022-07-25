#pramga once 

namespace TubeMoped
{
    class ToneStageProcessor
    {
    public:
        ToneStageProcessor(float samplerate, int blocksize, int channels)
        : samplerate(samplerate)
        {
            lowpass_.prepare(juce::dsp::ProcessSpec{sampleRate, static_cast<unsigned int>(samplesPerBlock), static_cast<unsigned int>(channels)});
            lowpass_.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
        }

        void process(juce::AudioBuffer<float>& buffer, float tone)
        {
            lowpass_.setCutoffFrequency(723.4f);

            auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
            auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
            lowpass_.process(context);
        }

    private:
        float samplerate_;

        juce::dsp::FirstOrderTPTFilter<float> lowpass_;
    }
}
