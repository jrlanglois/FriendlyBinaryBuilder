/*
    FriendlyBinaryBuilder: https://github.com/jrlanglois/FriendlyBinaryBuilder

    Copyright (C) 2013 by Joël R. Langlois <joel.r.langlois@gmail.com>

    This library contains portions of other open source products covered by
    separate licenses. Please see the corresponding source files for specific
    terms.
  
    FriendlyBinaryBuilder is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
#ifndef JRL_CUSTOM_LOOK_AND_FEEL_H
#define JRL_CUSTOM_LOOK_AND_FEEL_H

#include "JuceHeader.h"

class CustomLookAndFeel : public juce::LookAndFeel
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