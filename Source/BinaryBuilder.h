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
    BinaryBuilder (const juce::File& sourceDirectory,
                   const juce::File& destinationDirectory);

    /**
    * Destructor
    */
    ~BinaryBuilder();

    //==============================================================================
    void setSourceDirectory (const juce::File& sourceDirectory);

    void setDestinationDirectory (const juce::File& destinationDirectory);

    //==============================================================================
    void generateToFiles (const juce::String& className);

private:
    //==============================================================================
    juce::String className;
    juce::File sourceDirectory;
    juce::File destinationDirectory;

    //==============================================================================
    bool sourceFolderHasFiles();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinaryBuilder);
};

#endif //JRL_BINARY_BUILDER_H