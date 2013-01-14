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
#ifndef JRL_MAIN_COMPONENT_H
#define JRL_MAIN_COMPONENT_H

#include "BinaryBuilder.h"
#include "FileListComponent.h"

class MainComponent : public juce::Component,
                      private juce::Button::Listener
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
    void resized();
    /** @internal */
    void paint (juce::Graphics& g);
    /** @internal */
    void buttonClicked (juce::Button* button);

private:
    //==============================================================================
    BinaryBuilder binaryBuilder;

    FileListComponent currentFiles;

    juce::TextButton destDirSelector;
    juce::Label destDirectory;

    juce::Label className;
    juce::TextEditor classNameEditor;

    juce::TextButton generate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};

#endif //JRL_MAIN_COMPONENT_H