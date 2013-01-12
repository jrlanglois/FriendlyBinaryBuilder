#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);
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
}