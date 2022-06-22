#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Processor.h"
#include "MainComponent.h"

namespace TubeMoped
{
    class Editor : public juce::AudioProcessorEditor
    {
    public:
        explicit Editor(Processor&);

        ~Editor() override = default;

        void resized() override;

    private:
        MainComponent mainComponent_;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
    };
}
