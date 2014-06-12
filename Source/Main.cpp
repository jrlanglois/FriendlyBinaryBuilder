#include "MainWindow.h"
#include "CustomLookAndFeel.h"

#ifdef JUCE_WINDOWS
    #ifdef JUCE_MSVC
        #ifdef JUCE_VC8_OR_EARLIER
            #error "Message from Joel: Please use the latest Visual Studio possible. The Express version is free!"
        #endif
    #endif
#endif

class FriendlyBinaryBuilderApplication : public juce::JUCEApplication
{
public:
    FriendlyBinaryBuilderApplication() { }

    //==============================================================================
    const juce::String getApplicationName() override    { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override          { return false; }

    //==============================================================================
    void initialise (const juce::String&) override
    {
        juce::LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
        mainWindow = new MainWindow();
    }

    void shutdown() override
    {
        mainWindow = nullptr;
        juce::LookAndFeel::setDefaultLookAndFeel (nullptr);
    }

private:
    //==============================================================================
    juce::ScopedPointer<MainWindow> mainWindow;
    juce::LookAndFeel_V3 lookAndFeel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FriendlyBinaryBuilderApplication)
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (FriendlyBinaryBuilderApplication)