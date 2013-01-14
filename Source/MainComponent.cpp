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

    destDirSelector.setButtonText ("Change directory...");
    destDirectory.setText (juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName(), false);

    className.setText ("Class name:", false);
    classNameEditor.setText (juce::String::empty, false);

    generate.setButtonText ("Generate...");

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
    currentFiles.setBounds (50, 50, getWidth() - 100, getHeight() - 100);

    destDirSelector.setBounds (0, 0, 100, 100);
    destDirectory.setBounds (0, 0, 100, 100);
    className.setBounds (0, 0, 100, 100);
    classNameEditor.setBounds (0, 0, 100, 100);
    generate.setBounds (0, 0, 100, 100);
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &destDirSelector)
    {
        
    }
    else if (button == &generate)
    {
        binaryBuilder.setDestinationDirectory (destDirectory.getText().trim());
        binaryBuilder.generateBinaries (classNameEditor.getText().trim());
    }
}