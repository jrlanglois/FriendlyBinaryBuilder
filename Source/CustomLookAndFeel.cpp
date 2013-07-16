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

//==============================================================================
void CustomLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                              juce::Button& button,
                                              const juce::Colour& backgroundColour,
                                              const bool isMouseOverButton,
                                              const bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();

    const float outlineThickness = button.isEnabled() ? ((isButtonDown || isMouseOverButton) ? 1.2f : 0.7f) : 0.4f;
    const float halfThickness = outlineThickness * 0.5f;

    const float indentL = button.isConnectedOnLeft()   ? 0.1f : halfThickness;
    const float indentR = button.isConnectedOnRight()  ? 0.1f : halfThickness;
    const float indentT = button.isConnectedOnTop()    ? 0.1f : halfThickness;
    const float indentB = button.isConnectedOnBottom() ? 0.1f : halfThickness;

    const juce::Colour baseColour (createBaseColour (backgroundColour,
                                                     button.hasKeyboardFocus (true),
                                                     isMouseOverButton, isButtonDown)
                                   .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    juce::LookAndFeel::drawGlassLozenge (g,
                                         indentL,
                                         indentT,
                                         width - indentL - indentR,
                                         height - indentT - indentB,
                                         baseColour, outlineThickness, -1.0f,
                                         button.isConnectedOnLeft(),
                                         button.isConnectedOnRight(),
                                         button.isConnectedOnTop(),
                                         button.isConnectedOnBottom());
}

void CustomLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                          const bool isMouseOverButton, const bool isButtonDown)
{
    const float fontSize = juce::jmin (15.0f, button.getHeight() * 0.75f);
    const float tickWidth = fontSize * 1.1f;

    drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                 tickWidth, tickWidth,
                 button.getToggleState(),
                 button.isEnabled(),
                 isMouseOverButton,
                 isButtonDown);

    g.setColour (button.findColour (juce::ToggleButton::textColourId));
    g.setFont (fontSize);

    if (! button.isEnabled())
        g.setOpacity (0.5f);

    const int textX = (int) tickWidth + 5;

    g.drawFittedText (button.getButtonText(),
                      textX, 0,
                      button.getWidth() - textX - 2, button.getHeight(),
                      juce::Justification::centredLeft, 10);
}