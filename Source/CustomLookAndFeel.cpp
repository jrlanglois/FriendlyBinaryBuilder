#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    const juce::Colour textColour (juce::Colours::white);

    setColour (juce::ToggleButton::textColourId, textColour);
    setColour (juce::Label::textColourId, textColour);
    setColour (juce::GroupComponent::textColourId, textColour);
    setColour (juce::ListBox::textColourId, textColour);
}