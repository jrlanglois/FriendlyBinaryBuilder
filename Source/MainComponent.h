#ifndef JRL_MAIN_COMPONENT_H
#define JRL_MAIN_COMPONENT_H

#include "BinaryBuilder.h"
#include "FileListComponent.h"

class MainComponent : public juce::Component,
                      private juce::Button::Listener //Sorry - I don't believe in using VS2005 in today's day and age
{
public:
    /** Constructor */
    MainComponent();

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (juce::Graphics& g) override;
    /** @internal */
    void buttonClicked (juce::Button* button) override;

private:
    //==============================================================================
    BinaryBuilder binaryBuilder;
    FileListComponent currentFiles;

    juce::Label className;
    juce::ToggleButton alwaysUseUnsigned, zipDataStreams;
    juce::TextEditor destDirectory, classNameEditor;
    juce::TextButton destDirSelector, clearAll, generate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};

#endif //JRL_MAIN_COMPONENT_H