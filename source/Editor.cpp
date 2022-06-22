#include "Editor.h"

namespace TubeMoped
{
    Editor::Editor(Processor& p)
    : AudioProcessorEditor(&p)
    , mainComponent_(p.getAudioProcessorValueTreeState())
    {
        setSize(400, 300);
        addAndMakeVisible(mainComponent_);
    }

    void Editor::resized()
    {
        mainComponent_.setBounds(getLocalBounds());
    }
}
