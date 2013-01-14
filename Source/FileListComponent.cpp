#include "FileListComponent.h"

FileListComponent::FileListComponent()
{
    fileListBox.setModel (this);
    fileListBox.setMultipleSelectionEnabled (true);
    fileListBox.setOutlineThickness (3);
    fileListBox.setColour (juce::ListBox::backgroundColourId, juce::Colours::darkgrey.brighter().withAlpha (0.5f));
    fileListBox.setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.25f));
    fileListBox.setColour (juce::ListBox::textColourId, juce::Colours::lightgrey);

    addAndMakeVisible (&fileListBox);
}

FileListComponent::~FileListComponent()
{
}

//==============================================================================
juce::Array<juce::File> FileListComponent::getFiles() const
{
    return files;
}

//==============================================================================
void FileListComponent::resized()
{
    fileListBox.setBounds (getBounds().withPosition (0, 0));
}

void FileListComponent::paint (juce::Graphics& /*g*/)
{
}

bool FileListComponent::isInterestedInFileDrag (const juce::StringArray& /*files*/)
{
    return true;
}

void FileListComponent::filesDropped (const juce::StringArray& incomingFiles, int /*x*/, int /*y*/)
{
    for (int i = 0; i < incomingFiles.size(); ++i)
    {
        const juce::File file (incomingFiles[i]);

        if (! isSourceControlFile (file)
           && file.existsAsFile())
        {
            files.addIfNotAlreadyThere (file);
        }
    }

    fileListBox.updateContent();
}

int FileListComponent::getNumRows()
{
    return files.size();
}

void FileListComponent::paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    const juce::Colour textColour (fileListBox.findColour (juce::ListBox::textColourId));

    if (rowIsSelected)
    {
        g.setColour (textColour.darker().darker());
        g.fillAll (juce::Colours::white.darker());
    }
    else
    {
        g.setColour (textColour);
    }

    g.setFont (height * 0.7f);

    g.drawText (files[rowNumber].getFullPathName(),
                5, 0, width, height,
                juce::Justification::centredLeft, false);
}

void FileListComponent::backgroundClicked()
{
    fileListBox.setSelectedRows (juce::SparseSet<int>());
}

void FileListComponent::deleteKeyPressed (int lastRowSelected)
{
    juce::SparseSet<int> selectedRows (fileListBox.getSelectedRows());

    for (int i = 0; i < selectedRows.size(); ++i)
    {
        files.remove (selectedRows[i]);
    }

    fileListBox.updateContent();
}

//==============================================================================
bool FileListComponent::isSourceControlFile (const juce::File& file)
{
    return file.getFileName().endsWithIgnoreCase (".scc")
        || file.getFileName() == ".svn"
        || file.getFileName() == ".gitignore";
}