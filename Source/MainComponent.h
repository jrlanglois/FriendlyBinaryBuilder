#ifndef JRL_MAIN_COMPONENT_H
#define JRL_MAIN_COMPONENT_H

#include "BinaryBuilder.h"
#include "FileListComponent.h"

class MainComponent : public juce::Component
{
public:
    /**
    * Constructor
    */
    MainComponent();

    /**
    * Destructor
    */
    ~MainComponent();

    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    void paint (juce::Graphics& g);

private:
    //==============================================================================
    BinaryBuilder binaryBuilder;

    FileListComponent currentFiles;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};

#endif //JRL_MAIN_COMPONENT_H