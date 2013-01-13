#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);

    binaryBuilder.addFile (juce::File ("C:/RHDSetup.log"));
    binaryBuilder.addFile (juce::File ("C:/asdf.asdf"));
    binaryBuilder.setDestinationDirectory (juce::File ("Z:/Bostwich"));
    binaryBuilder.generateBinaries ("asdf");
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