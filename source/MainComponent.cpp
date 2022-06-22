#include "MainComponent.h"

namespace TubeMoped
{
   MainComponent::MainComponent(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
    : audioProcessorValueTreeState_(audioProcessorValueTreeState)
    //, frequencySliderAttachment_(audioProcessorValueTreeState_, "lowpass cutoff frequency", dynamic_cast<dearUI::Knob&>(*components_["knob"]))
    {

    }
}
