#include "FileListComponent.h"

FileListComponent::FileListComponent()
{
    setModel (this);
    setMultipleSelectionEnabled (true);
    setOutlineThickness (2);
    setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
    setColour (juce::ListBox::textColourId, juce::Colours::white);
    setColour (juce::ListBox::backgroundColourId, juce::Colours::darkgrey.brighter().withAlpha (0.5f));

    juce::File file ("E:/Programming/FriendlyBinaryBuilder/Builds/VisualStudio2012/FriendlyBinaryBuilder.vcxproj");
    addFileIfValid (file);

    file = juce::File ("E:/Programming/FriendlyBinaryBuilder/Builds/VisualStudio2012/FriendlyBinaryBuilder.vcxproj.filters");
    addFileIfValid (file);

    updateContent();
}

//==============================================================================
void FileListComponent::clearAll()
{
    files.clear();
    updateContent();
}

//==============================================================================
bool FileListComponent::isInterestedInFileDrag (const juce::StringArray& /*files*/)
{
    return true; //Accept all files. Any invalid ones will be filtered later...
}

void FileListComponent::filesDropped (const juce::StringArray& incomingFiles, const int /*x*/, const int /*y*/)
{
    //Add valid files:
    for (int i = 0; i < incomingFiles.size(); ++i)
        addFileIfValid (incomingFiles[i]);

    //Update the table:
    updateContent();

    setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
}

void FileListComponent::fileDragEnter (const juce::StringArray& /*files*/, const int /*x*/, const int /*y*/)
{
    setColour (juce::ListBox::outlineColourId, juce::Colours::red.withAlpha (0.5f));
}

void FileListComponent::fileDragExit (const juce::StringArray& /*files*/)
{
    setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
}

//==============================================================================
int FileListComponent::getNumRows()
{
    return files.size();
}

void FileListComponent::paintListBoxItem (const int rowNumber, juce::Graphics& g,
                                          const int width, const int height, const bool isRowSelected)
{
    const juce::Colour textColour (ListBox::findColour (juce::ListBox::textColourId));

    if (isRowSelected)
    {
        g.setColour (textColour.darker().darker());
        g.fillAll (juce::Colours::white.darker());
    }
    else
    {
        g.setColour (textColour);
    }

    g.setFont (height * 0.7f);

    g.drawText (files.getReference (rowNumber).getFullPathName(),
                5, 0, width, height,
                juce::Justification::centredLeft, false);
}

void FileListComponent::backgroundClicked (const juce::MouseEvent&)
{
    setSelectedRows (juce::SparseSet<int>());
}

void FileListComponent::deleteKeyPressed (const int)
{
    juce::SparseSet<int> selectedRows (getSelectedRows());

    for (int i = selectedRows.size(); --i >= 0;)
        files.remove (selectedRows[i]);

    updateContent();
}

void FileListComponent::listBoxItemClicked (const int row, const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        enum Options
        {
            Remove = 1,
            ShowInFolder
        };

        juce::PopupMenu popup;
        popup.addItem (Remove, "Remove");
        popup.addItem (ShowInFolder, "Show in folder...");

        switch (popup.show())
        {
            case Remove:
                files.remove (row);
                updateContent();
            break;

            case ShowInFolder:
                files.getReference (row).revealToUser();
            break;

            default:
            break;
        }
    }
}

//==============================================================================
bool FileListComponent::isSourceControlFile (const juce::File& file) const
{
    return file.getFileName().endsWithIgnoreCase (".scc")
           || file.getFileName() == ".svn"
           || file.getFileName() == ".gitignore";
}

void FileListComponent::addFileIfValid (const juce::File& file)
{
    if (file.isDirectory())
    {
        addRecursively (file);
    }
    else
    {
        if (! isSourceControlFile (file) && file.existsAsFile())
            files.addIfNotAlreadyThere (file);
    }
}

void FileListComponent::addRecursively (const juce::File& directory)
{
    juce::Array<juce::File> files;
    directory.findChildFiles (files, juce::File::findFilesAndDirectories, true);

    for (int i = 0; i < files.size(); ++i)
        addFileIfValid (files.getReference (i));
}