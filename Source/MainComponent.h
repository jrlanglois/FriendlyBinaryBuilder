#ifndef JRL_MAIN_COMPONENT_H
#define JRL_MAIN_COMPONENT_H

#include "BinaryBuilder.h"

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
    void paint (juce::Graphics& g);
    /** @internal */
    void resized();

private:
    //==============================================================================
    BinaryBuilder binaryBuilder;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};

#endif //JRL_MAIN_COMPONENT_H