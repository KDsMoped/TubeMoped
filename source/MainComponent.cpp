#include "MainComponent.h"

namespace TubeMoped
{
   MainComponent::MainComponent(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
    : audioProcessorValueTreeState_(audioProcessorValueTreeState)
    , distortionKnobAttachment_(audioProcessorValueTreeState_, "Distortion", distortionKnob_)
    , toneKnobAttachment_(audioProcessorValueTreeState_, "Tone", toneKnob_)
    {
        distortionKnob_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        distortionKnob_.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
        addAndMakeVisible(distortionKnob_);

        toneKnob_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        toneKnob_.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
        addAndMakeVisible(toneKnob_);
    }
    
    void MainComponent::resized()
    {
        distortionKnob_.setBounds(0, 0, 100, 150);
        distortionKnob_.setCentrePosition(getLocalBounds().removeFromLeft(getWidth()/2).getCentre());

        toneKnob_.setBounds(0, 0, 100, 150);
        toneKnob_.setCentrePosition(getLocalBounds().removeFromRight(getWidth()/2).getCentre());
    }
}
