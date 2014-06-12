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

    //==============================================================================
    const juce::Array<juce::File>& getFiles() const noexcept { return files; }

    void clearAll();

    //==============================================================================
    /** @internal */
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    /** @internal */
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    /** @internal */
    void fileDragEnter (const juce::StringArray& files, int x, int y) override;
    /** @internal */
    void fileDragExit (const juce::StringArray& files) override;
    /** @internal */
    int getNumRows() override;
    /** @internal */
    void paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    /** @internal */
    void backgroundClicked (const juce::MouseEvent& e) override;
    /** @internal */
    void deleteKeyPressed (int lastRowSelected) override;
    /** @internal */
    void listBoxItemClicked (int row, const juce::MouseEvent& e) override;

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