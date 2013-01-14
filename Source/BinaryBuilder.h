#ifndef JRL_BINARY_BUILDER_H
#define JRL_BINARY_BUILDER_H

#include "JuceHeader.h"

/**
* @file BinaryBuilder.h
*
* Simple class used to generate header and cpp files containing binary
* data that was based of files located in the source directory.
*
* @authors Joël R. Langlois, Julian Storer (credited for original code)
*/
class BinaryBuilder
{
public:
    /**
    * Constructor
    */
    BinaryBuilder();

    /**
    * Constructor
    */
    BinaryBuilder (const juce::File& destinationDirectory);

    /**
    * Destructor
    */
    ~BinaryBuilder() noexcept;

    //==============================================================================
    void addFile (const juce::File& file);
    void addFiles (const juce::Array<juce::File>& files);
    void clear();

    void setDestinationDirectory (const juce::File& destinationDirectory);

    //==============================================================================
    void generateBinaries (const juce::String& className = BinaryBuilder::defaultClassName);

    //==============================================================================
    static const juce::String defaultClassName;

private:
    //==============================================================================
    juce::File destinationDirectory;
    juce::Array<juce::File> files;

    //==============================================================================
    juce::String createValidVersionOfClassName (const juce::String& className) const;

    //==============================================================================
    void removeNonexistentFiles();

    bool hasValidFiles();

    //==============================================================================
    bool destinationDirectoryExists();

    bool hasValidDestinationDirectory();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinaryBuilder);
};

#endif //JRL_BINARY_BUILDER_H