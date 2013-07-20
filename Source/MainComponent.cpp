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
    setSize (600, 500);

    const juce::Colour buttonColour (juce::Colours::blue.brighter().brighter());

    destDirSelector.setButtonText ("Change directory...");
    destDirSelector.addListener (this);

#ifdef JUCE_WINDOWS
    destDirectory.setText (juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName() + "\\Generated Binary Files\\", false);
#else
    destDirectory.setText (juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName() + "/Generated Binary Files/", false);
#endif

    className.setText ("Namespace name:", juce::dontSendNotification);
    className.setJustificationType (juce::Justification::centredRight);
    className.setColour (juce::Label::textColourId, juce::Colours::white);
    classNameEditor.setText (BinaryBuilder::defaultClassName, false);

    {
        const juce::String chars ("abcdefghijklmnopqrstuvwxyz");
        classNameEditor.setInputRestrictions (0, "1234567890-_" + chars.toLowerCase() + chars.toUpperCase());
    }

    alwaysUseUnsigned.setButtonText ("Always use unsigned for arrays");
    zipDataStreams.setButtonText ("Zip data (uses JUCE/ZLib)");

    clearAll.setButtonText ("Clear all");
    clearAll.addListener (this);

    generate.setButtonText ("Generate...");
    generate.addListener (this);

    addAndMakeVisible (&currentFiles);
    addAndMakeVisible (&destDirSelector);
    addAndMakeVisible (&destDirectory);
    addAndMakeVisible (&className);
    addAndMakeVisible (&classNameEditor);
    addAndMakeVisible (&alwaysUseUnsigned);
    addAndMakeVisible (&zipDataStreams);
    addAndMakeVisible (&clearAll);
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
                            getHeight() - (normalCompHeight * 5) - doubleBorderThickness);

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

        classNameEditor.setBounds (className.getRight() + borderThickness, compY,
                                   destDirectory.getWidth(), normalCompHeight);
    }

    {
        alwaysUseUnsigned.setBounds (borderThickness, className.getBottom() + borderThickness,
                                     200, normalCompHeight);

        zipDataStreams.setBounds (alwaysUseUnsigned.getBounds().withX (alwaysUseUnsigned.getRight()));
    }

    {
        generate.setSize (normalCompWidth, normalCompHeight);
        generate.setTopRightPosition (getWidth() - borderThickness,
                                      alwaysUseUnsigned.getBottom() + borderThickness);

        clearAll.setBounds (generate.getBounds());
        clearAll.setTopLeftPosition (clearAll.getX() - normalCompWidth - borderThickness, clearAll.getY());
    }
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &destDirSelector)
    {
        juce::FileChooser chooser ("Select a new folder");

        if (chooser.browseForDirectory())
            destDirectory.setText (chooser.getResult().getFullPathName(), true);
    }
    else if (button == &clearAll)
    {
        if (currentFiles.getNumRows() > 0)
        {
            const int result = juce::AlertWindow::showYesNoCancelBox (juce::AlertWindow::InfoIcon,
                                                                      "Clear the files?",
                                                                      "Are you sure you want to clear all the listed files?",
                                                                      "Yes", "No");
            if (result == -1)
                currentFiles.clearAll();
        }
    }
    else if (button == &generate)
    {
        binaryBuilder.clear();
        binaryBuilder.setDestinationDirectory (destDirectory.getText().trim());
        binaryBuilder.addFiles (currentFiles.getFiles());

        binaryBuilder.generateBinaries (alwaysUseUnsigned.getToggleState(),
                                        zipDataStreams.getToggleState(),
                                        classNameEditor.getText().trim());
    }
}