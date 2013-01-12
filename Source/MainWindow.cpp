#include "MainWindow.h"

MainWindow::MainWindow() :
    juce::DocumentWindow ("Binary Builder",
                          juce::Colours::darkgrey,
                          juce::DocumentWindow::closeButton)
{
    setContentOwned (new MainComponent(), true);

    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}

MainWindow::~MainWindow()
{
}

//==============================================================================
void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}