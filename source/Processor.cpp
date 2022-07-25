#include "Processor.h"

#include "Editor.h"

namespace TubeMoped
{
    namespace
    {
        juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
        {
            auto layout = juce::AudioProcessorValueTreeState::ParameterLayout();
            layout.add(std::make_unique<juce::AudioParameterInt>("Distortion", "Distortion", 0, 100, 50));
            layout.add(std::make_unique<juce::AudioParameterFloat>("Tone", "Tone", 0.f, 1.f, 0.5f));
            layout.add(std::make_unique<juce::AudioParameterInt>("Level", "Level", 0, 100, 50));
            return layout;
        }
    }

    Processor::Processor()
    : AudioProcessor(BusesProperties() .withInput("Input", juce::AudioChannelSet::stereo(), true) .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , apvts_(*this, nullptr, "PARAMETERS", createParameterLayout())
    {
    }

    juce::AudioProcessorValueTreeState& Processor::getAudioProcessorValueTreeState()
    {
        return apvts_;
    }

    const juce::String Processor::getName() const{ return JucePlugin_Name; }
    bool Processor::acceptsMidi() const { return false; }
    bool Processor::producesMidi() const {  return false; }
    bool Processor::isMidiEffect() const { return false; }
    double Processor::getTailLengthSeconds() const { return 0.0; }
    int Processor::getNumPrograms() { return 1; }
    int Processor::getCurrentProgram() { return 0; }
    void Processor::setCurrentProgram(int ){}
    const juce::String Processor::getProgramName(int ){ return {};}
    void Processor::changeProgramName(int , const juce::String& ){}

    void Processor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        clippingStageProcessor_ = std::make_unique<ClippingStageProcessor>(sampleRate, samplesPerBlock, getNumInputChannels());
        toneStageProcessor_ = std::make_unique<ToneStageProcessor>(sampleRate, samplesPerBlock, getNumInputChannels());
    }

    void Processor::releaseResources()
    {
    }

    bool Processor::isBusesLayoutSupported(const BusesLayout& layouts) const
    {
        if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        return true;
    }

    void Processor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& )
    {
        auto distValue = apvts_.getParameter("Distortion")->getValue();
        clippingStageProcessor_->process(buffer, distValue);
        
        auto toneValue = apvts_.getParameter("Tone")->getValue();
        toneStageProcessor_->process(buffer, toneValue);

        buffer.applyGain(.2f);
    }

    bool Processor::hasEditor() const
    {
        return true;
    }

    juce::AudioProcessorEditor* Processor::createEditor()
    {
        return new Editor(*this);
    }

    void Processor::getStateInformation(juce::MemoryBlock& destData)
    {
        const auto state = apvts_.copyState();
        const auto xml = std::unique_ptr<juce::XmlElement>(state.createXml());
        copyXmlToBinary (*xml, destData);
    }

    void Processor::setStateInformation(const void* data, int sizeInBytes)
    {
        const auto xmlState = std::unique_ptr<juce::XmlElement>(getXmlFromBinary(data, sizeInBytes));

        if(xmlState != nullptr)
            if (xmlState->hasTagName(apvts_.state.getType()))
                apvts_.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TubeMoped::Processor();
}
