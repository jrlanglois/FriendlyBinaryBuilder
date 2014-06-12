#ifndef JRL_CUSTOM_LOOK_AND_FEEL_H
#define JRL_CUSTOM_LOOK_AND_FEEL_H

#include "JuceHeader.h"

class CustomLookAndFeel : public juce::LookAndFeel_V3
{
public:
    /** Constructor */
    CustomLookAndFeel();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};

#endif //JRL_CUSTOM_LOOK_AND_FEEL_H