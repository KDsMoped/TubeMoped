#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

namespace TubeMoped
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState);

        ~MainComponent() override = default;

        void resized() override;
    private:
        juce::AudioProcessorValueTreeState& audioProcessorValueTreeState_;
        
        juce::Slider distortionKnob_;
        juce::AudioProcessorValueTreeState::SliderAttachment distortionKnobAttachment_;

        juce::Slider toneKnob_;
        juce::AudioProcessorValueTreeState::SliderAttachment toneKnobAttachment_;
    };
}
