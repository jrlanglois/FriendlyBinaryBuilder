#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (700, 500);

    const juce::Colour buttonColour (juce::Colours::blue.brighter().brighter());

    destDirSelector.setButtonText ("Change Directory...");
    destDirSelector.addListener (this);

   #ifdef JUCE_WINDOWS
    destDirectory.setText (juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName() + "\\Generated Binary Files\\", false);
   #else
    destDirectory.setText (juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName() + "/Generated Binary Files/", false);
   #endif

    className.setText ("File Name:", juce::dontSendNotification);
    className.setJustificationType (juce::Justification::centredRight);
    className.setColour (juce::Label::textColourId, juce::Colours::white);
    classNameEditor.setText (BinaryBuilder::defaultClassName, false);

    {
        const juce::String chars ("abcdefghijklmnopqrstuvwxyz");
        classNameEditor.setInputRestrictions (0, "1234567890-_" + chars.toLowerCase() + chars.toUpperCase());
    }

    alwaysUseUnsigned.setButtonText ("Always use unsigned for arrays");
    zipDataStreams.setButtonText ("Zip data (uses JUCE/ZLib)");

    alwaysUseUnsigned.setColour (juce::ToggleButton::textColourId, juce::Colours::white);
    zipDataStreams.setColour (juce::ToggleButton::textColourId, juce::Colours::white);

    clearAll.setButtonText ("Clear all");
    clearAll.addListener (this);

    generate.setButtonText ("Generate...");
    generate.addListener (this);

    addAndMakeVisible (&currentFiles);
    addAndMakeVisible (&destDirSelector);
    addAndMakeVisible (&destDirectory);
    addAndMakeVisible (&className);
    addAndMakeVisible (&classNameEditor);
    addAndMakeVisible (&alwaysUseUnsigned);
    addAndMakeVisible (&zipDataStreams);
    addAndMakeVisible (&clearAll);
    addAndMakeVisible (&generate);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    const int width                 = getWidth();
    const int normalCompWidth       = 150;
    const int normalCompHeight      = 25;
    const int borderThickness       = 5;
    const int doubleBorderThickness = borderThickness * 2;

    currentFiles.setBounds (borderThickness,
                            borderThickness,
                            getWidth() - doubleBorderThickness,
                            getHeight() - (normalCompHeight * 5) - doubleBorderThickness);

    {
        const int compY = currentFiles.getBottom() + borderThickness;

        destDirSelector.setBounds (borderThickness, compY, normalCompWidth, normalCompHeight);

        destDirectory.setBounds (destDirSelector.getRight() + borderThickness,
                                 compY,
                                 currentFiles.getRight() - destDirSelector.getRight() - borderThickness,
                                 normalCompHeight);
    }

    {
        const int compY = destDirSelector.getBottom() + doubleBorderThickness;

        className.setBounds (borderThickness, compY, normalCompWidth, normalCompHeight);
        classNameEditor.setBounds (destDirectory.getX(), compY, destDirectory.getWidth() / 2, normalCompHeight);

        classNameEditor.setBounds (className.getRight() + borderThickness, compY,
                                   destDirectory.getWidth(), normalCompHeight);
    }

    {
        const int toggleWidth = width / 2;

        alwaysUseUnsigned.setBounds (borderThickness, className.getBottom() + borderThickness,
                                     toggleWidth, normalCompHeight);

        zipDataStreams.setBounds (alwaysUseUnsigned.getBounds().withX (alwaysUseUnsigned.getRight()));
    }

    {
        generate.setSize (normalCompWidth, normalCompHeight);
        generate.setTopRightPosition (getWidth() - borderThickness,
                                      alwaysUseUnsigned.getBottom() + borderThickness);

        clearAll.setBounds (generate.getBounds());
        clearAll.setTopLeftPosition (clearAll.getX() - normalCompWidth - borderThickness, clearAll.getY());
    }
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &destDirSelector)
    {
        juce::FileChooser chooser ("Select a new folder");

        if (chooser.browseForDirectory())
            destDirectory.setText (chooser.getResult().getFullPathName(), true);
    }
    else if (button == &clearAll)
    {
        if (currentFiles.getNumRows() > 0)
        {
            const int result = juce::AlertWindow::showYesNoCancelBox (juce::AlertWindow::InfoIcon,
                                                                      "Clear the files?",
                                                                      "Are you sure you want to clear all the listed files?",
                                                                      "Yes", "No");

            if (result == -1)
                currentFiles.clearAll();
        }
    }
    else if (button == &generate)
    {
        binaryBuilder.clear();
        binaryBuilder.setDestinationDirectory (destDirectory.getText().trim());
        binaryBuilder.addFiles (currentFiles.getFiles());

        binaryBuilder.generateBinaries (alwaysUseUnsigned.getToggleState(),
                                        zipDataStreams.getToggleState(),
                                        classNameEditor.getText().trim());
    }
}