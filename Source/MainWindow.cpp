#include "MainWindow.h"

MainWindow::MainWindow() :
    juce::DocumentWindow ("Friendly Binary Builder",
                          juce::Colours::darkgrey,
                          juce::DocumentWindow::closeButton)
{
    setUsingNativeTitleBar (true);
    setContentOwned (new MainComponent(), true);

    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}

//==============================================================================
void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}