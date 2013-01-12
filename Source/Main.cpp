#include "MainWindow.h"

class FriendlyBinaryBuilderApplication : public juce::JUCEApplication
{
public:
    FriendlyBinaryBuilderApplication()  { }
    ~FriendlyBinaryBuilderApplication() { }

    //==============================================================================
    const juce::String getApplicationName()     { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion()  { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()           { return true; }

    //==============================================================================
    void initialise (const juce::String& /*commandLine*/)
    {
        mainWindow = new MainWindow();
    }

    void shutdown()
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        quit();
    }

    void anotherInstanceStarted (const juce::String& /*commandLine*/)
    {
    }

private:
    //==============================================================================
    juce::ScopedPointer<MainWindow> mainWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FriendlyBinaryBuilderApplication)
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (FriendlyBinaryBuilderApplication)