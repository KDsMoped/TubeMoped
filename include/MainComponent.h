#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

namespace TubeMoped
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState);

        ~MainComponent() override = default;

    private:
        juce::AudioProcessorValueTreeState& audioProcessorValueTreeState_;
        //juce::AudioProcessorValueTreeState::SliderAttachment frequencySliderAttachment_;
    };
}
