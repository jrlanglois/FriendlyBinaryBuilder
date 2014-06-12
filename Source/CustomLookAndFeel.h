#ifndef JRL_CUSTOM_LOOK_AND_FEEL_H
#define JRL_CUSTOM_LOOK_AND_FEEL_H

#include "JuceHeader.h"

class CustomLookAndFeel/* : public juce::LookAndFeel*/
{
public:
    /** Constructor */
    CustomLookAndFeel();

    /** Destructor */
    ~CustomLookAndFeel();

    //==============================================================================
    juce::Font getTextButtonFont (juce::TextButton& button);

    juce::Font getComboBoxFont (juce::ComboBox& box);

    juce::Font getLabelFont (juce::Label& label);

    juce::Font getAlertWindowMessageFont();
    juce::Font getAlertWindowFont();
    juce::Font getPopupMenuFont();
    juce::Font getMenuBarFont (juce::MenuBarComponent& menuBar, int itemIndex, const juce::String& itemText);

    //==============================================================================
    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown);

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool isMouseOverButton, bool isButtonDown);

private:
    //==============================================================================
    static juce::Colour createBaseColour (const juce::Colour& buttonColour,
                                          const bool hasKeyboardFocus,
                                          const bool isMouseOverButton,
                                          const bool isButtonDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const juce::Colour baseColour (buttonColour.withMultipliedSaturation (sat));

        if (isButtonDown)      { return baseColour.contrasting (0.2f); }
        if (isMouseOverButton) { return baseColour.contrasting (0.1f); }

        return baseColour;
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};

#endif //JRL_CUSTOM_LOOK_AND_FEEL_H