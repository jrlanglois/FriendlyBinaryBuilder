/*
    FriendlyBinaryBuilder: https://github.com/jrlanglois/FriendlyBinaryBuilder

    Copyright (C) 2013 by Joël R. Langlois <joel.r.langlois@gmail.com>

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
#ifndef JRL_FILE_LIST_COMPONENT_H
#define JRL_FILE_LIST_COMPONENT_H

#include "JuceHeader.h"

class FileListComponent : public juce::ListBoxModel,
                          public juce::ListBox,
                          public juce::FileDragAndDropTarget
{
public:
    /** Constructor */
    FileListComponent();

    /** Destructor */
    ~FileListComponent();

    //==============================================================================
    const juce::Array<juce::File>& getFiles() const noexcept { return files; }

    void clearAll();

    //==============================================================================
    /** @internal */
    bool isInterestedInFileDrag (const juce::StringArray& files);
    /** @internal */
    void filesDropped (const juce::StringArray& files, int x, int y);
    /** @internal */
    void fileDragEnter (const juce::StringArray& files, int x, int y);
    /** @internal */
    void fileDragExit (const juce::StringArray& files);
    /** @internal */
    int getNumRows();
    /** @internal */
    void paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected);
    /** @internal */
    void backgroundClicked();
    /** @internal */
    void deleteKeyPressed (int lastRowSelected);
    /** @internal */
    void listBoxItemClicked (int row, const juce::MouseEvent& e);

private:
    //==============================================================================
    juce::Array<juce::File> files;

    //==============================================================================
    bool isSourceControlFile (const juce::File& file) const;

    void addFileIfValid (const juce::File& file);

    void addRecursively (const juce::File& directory);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComponent);
};

#endif //JRL_FILE_LIST_COMPONENT_H