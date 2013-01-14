#ifndef JRL_FILE_LIST_COMPONENT_H
#define JRL_FILE_LIST_COMPONENT_H

#include "JuceHeader.h"

class FileListComponent : public juce::Component,
                          public juce::FileDragAndDropTarget,
                          private juce::ListBoxModel
{
public:
    /**
    * Constructor
    */
    FileListComponent();

    /**
    * Destructor
    */
    ~FileListComponent();

    //==============================================================================
    juce::Array<juce::File> getFiles() const;

    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    void paint (juce::Graphics& g);
    /** @internal */
    bool isInterestedInFileDrag (const juce::StringArray& files);
    /** @internal */
    void filesDropped (const juce::StringArray& files, int x, int y);
    /** @internal */
    int getNumRows();
    /** @internal */
    void paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected);
    /** @internal */
    void backgroundClicked();
    /** @internal */
    void deleteKeyPressed (int lastRowSelected);

private:
    //==============================================================================
    juce::ListBox fileListBox;
    juce::Array<juce::File> files;

    //==============================================================================
    static bool isSourceControlFile (const juce::File& file);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComponent);
};

#endif //JRL_FILE_LIST_COMPONENT_H