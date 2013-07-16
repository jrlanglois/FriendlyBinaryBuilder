/*
    FriendlyBinaryBuilder: https://github.com/jrlanglois/FriendlyBinaryBuilder

    Copyright (C) 2013 by Jo�l R. Langlois <joel.r.langlois@gmail.com>

    This library contains portions of other open source products covered by
    separate licenses. Please see the corresponding source files for specific
    terms.
  
    FriendlyBinaryBuilder is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
#include "FileListComponent.h"

FileListComponent::FileListComponent()
{
    setModel (this);
    setMultipleSelectionEnabled (true);
    setOutlineThickness (2);
    setColour (juce::ListBox::outlineColourId, juce::Colours::white.withAlpha (0.5f));
    setColour (juce::ListBox::textColourId, juce::Colours::lightgrey);
    setColour (juce::ListBox::backgroundColourId, juce::Colours::darkgrey.brighter().withAlpha (0.5f));
}

FileListComponent::~FileListComponent()
{
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
    {
        const juce::File file (incomingFiles[i]);

        if (file.isDirectory())
            addRecursively (file);
        else
            addFileIfValid (file);
    }

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

void FileListComponent::backgroundClicked()
{
    setSelectedRows (juce::SparseSet<int>());
}

void FileListComponent::deleteKeyPressed (const int /*lastRowSelected*/)
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
    if (! isSourceControlFile (file) && file.existsAsFile())
        files.addIfNotAlreadyThere (file);
}

void FileListComponent::addRecursively (const juce::File& directory)
{
    juce::Array<juce::File> files;
    directory.findChildFiles (files, juce::File::findFilesAndDirectories, true);

    for (int i = 0; i < files.size(); ++i)
        addFileIfValid (files.getReference (i));
}