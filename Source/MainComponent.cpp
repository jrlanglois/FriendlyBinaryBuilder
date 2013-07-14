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
#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);

    const juce::Colour buttonColour = juce::Colours::blue.brighter().brighter();

    destDirSelector.setColour (juce::TextButton::buttonColourId, buttonColour);
    destDirSelector.setColour (juce::TextButton::buttonOnColourId, buttonColour.brighter (0.75f));
    destDirSelector.setColour (juce::TextButton::textColourOffId, juce::Colours::black);
    destDirSelector.setColour (juce::TextButton::textColourOnId, juce::Colours::black);
    destDirSelector.setButtonText ("Change directory...");
    destDirSelector.addListener (this);

    destDirectory.setColour (juce::Label::textColourId, juce::Colours::lightgrey);
    destDirectory.setColour (juce::Label::outlineColourId, juce::Colours::lightgrey);
    destDirectory.setColour (juce::Label::backgroundColourId, juce::Colours::darkgrey);
    destDirectory.setText (juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName(), juce::dontSendNotification);
    destDirectory.setEditable (false, true, false);

    className.setText ("Class name:", juce::dontSendNotification);
    className.setJustificationType (juce::Justification::centredRight);
    className.setColour (juce::Label::textColourId, juce::Colours::white);
    classNameEditor.setText (BinaryBuilder::defaultClassName, false);

    {
        const juce::String chars = "abcdefghijklmnopqrstuvwxyz";
        classNameEditor.setInputRestrictions (0, "1234567890-_" + chars.toLowerCase() + chars.toUpperCase());
    }

    generate.setColour (juce::TextButton::buttonColourId, buttonColour);
    generate.setColour (juce::TextButton::buttonOnColourId, buttonColour.brighter (0.75f));
    generate.setColour (juce::TextButton::textColourOffId, juce::Colours::black);
    generate.setColour (juce::TextButton::textColourOnId, juce::Colours::black);
    generate.setButtonText ("Generate...");
    generate.addListener (this);

    addAndMakeVisible (&currentFiles);
    addAndMakeVisible (&destDirSelector);
    addAndMakeVisible (&destDirectory);
    addAndMakeVisible (&className);
    addAndMakeVisible (&classNameEditor);
    addAndMakeVisible (&generate);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    const int normalCompWidth       = 150;
    const int normalCompHeight      = 25;
    const int borderThickness       = 5;
    const int doubleBorderThickness = borderThickness * 2;

    currentFiles.setBounds (borderThickness,
                            borderThickness,
                            getWidth() - doubleBorderThickness,
                            getHeight() - (normalCompHeight * 3) - doubleBorderThickness);

    {
        const int compY = currentFiles.getBottom() + borderThickness;

        destDirSelector.setBounds (borderThickness, compY, normalCompWidth, normalCompHeight);

        destDirectory.setBounds (destDirSelector.getRight() + borderThickness,
                                 compY,
                                 currentFiles.getRight() - destDirSelector.getRight() - borderThickness,
                                 normalCompHeight);
    }

    {
        const int compY = destDirSelector.getBottom() + doubleBorderThickness;

        className.setBounds (borderThickness, compY, normalCompWidth, normalCompHeight);
        classNameEditor.setBounds (destDirectory.getX(), compY, destDirectory.getWidth() / 2, normalCompHeight);

        classNameEditor.setBounds (className.getRight() + borderThickness,
                                   compY,
                                   destDirectory.getWidth() - borderThickness - normalCompWidth,
                                   normalCompHeight);
    }

    generate.setSize (normalCompWidth, normalCompHeight);
    generate.setTopRightPosition (getWidth() - borderThickness, className.getY());
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &destDirSelector)
    {
        juce::FileChooser chooser ("Select a new folder");
        
        if (chooser.browseForDirectory())
            destDirectory.setText (chooser.getResult().getFullPathName(), juce::sendNotification);
    }
    else if (button == &generate)
    {
        binaryBuilder.setDestinationDirectory (destDirectory.getText().trim());

        binaryBuilder.clear();
        binaryBuilder.addFiles (currentFiles.getFiles());

        binaryBuilder.generateBinaries (classNameEditor.getText().trim());
    }
}