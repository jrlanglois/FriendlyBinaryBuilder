#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setDefaultSansSerifTypefaceName ("Calibri");
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

//==============================================================================
juce::Font CustomLookAndFeel::getTextButtonFont (juce::TextButton& button)
{
    return juce::Font ("Calibri", "Regular", button.getHeight() * 0.75f);
}

juce::Font CustomLookAndFeel::getComboBoxFont (juce::ComboBox& box)
{
    return juce::Font ("Calibri", "Regular", box.getHeight() * 0.75f);
}

juce::Font CustomLookAndFeel::getLabelFont (juce::Label& label)
{
    return juce::Font ("Calibri", "Regular", label.getHeight() * 0.75f);
}

juce::Font CustomLookAndFeel::getAlertWindowMessageFont()
{
    return juce::Font::getDefaultTypefaceForFont (juce::Font ("Calibri", "Regular", 10.0f));
}

juce::Font CustomLookAndFeel::getAlertWindowFont()
{
    return getAlertWindowMessageFont();
}

juce::Font CustomLookAndFeel::getPopupMenuFont()
{
    return getAlertWindowMessageFont();
}

juce::Font CustomLookAndFeel::getMenuBarFont (juce::MenuBarComponent& menuBar,
                                              const int /*itemIndex*/,
                                              const juce::String& /*itemText*/)
{
    return juce::Font ("Calibri", "Regular", menuBar.getHeight() * 0.75f);
}