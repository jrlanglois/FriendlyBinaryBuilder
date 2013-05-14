/*
    FriendlyBinaryBuilder: https://github.com/jrlanglois/FriendlyBinaryBuilder

    Copyright (C) 2013 by Jo�l R. Langlois <joel.r.langlois@gmail.com>

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
#include "MainWindow.h"
#include "CustomLookAndFeel.h"

class FriendlyBinaryBuilderApplication : public juce::JUCEApplication
{
public:
    FriendlyBinaryBuilderApplication() { }
    ~FriendlyBinaryBuilderApplication() { }

    //==============================================================================
    const juce::String getApplicationName()     { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion()  { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()           { return true; }

    //==============================================================================
    void initialise (const juce::String& /*commandLine*/)
    {
        juce::LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

        mainWindow = new MainWindow();
    }

    void shutdown()
    {
        mainWindow = nullptr;
    }

private:
    //==============================================================================
    juce::ScopedPointer<MainWindow> mainWindow;
    CustomLookAndFeel lookAndFeel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FriendlyBinaryBuilderApplication)
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (FriendlyBinaryBuilderApplication)