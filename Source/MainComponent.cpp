#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);
    addAndMakeVisible (&currentFiles);
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
}