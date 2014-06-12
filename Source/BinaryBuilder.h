#ifndef JRL_BINARY_BUILDER_H
#define JRL_BINARY_BUILDER_H

#include "JuceHeader.h"

/**
    @file BinaryBuilder.h
    @authors Joel R. Langlois, Julian Storer (credited for original code)

    Simple class used to generate header and cpp files containing binary
    data based off of the added files. The generated files will be placed
    in the set destination directory.
*/
class BinaryBuilder : private juce::AsyncUpdater
{
public:
    /** Constructor */
    BinaryBuilder();

    /** Constructor */
    BinaryBuilder (const juce::File& destinationDirectory);

    //==============================================================================
    void addFile (const juce::File& file);

    void addFiles (const juce::Array<juce::File>& files);

    void clear();

    void setDestinationDirectory (const juce::File& destinationDirectory);

    //==============================================================================
    void generateBinaries (bool alwaysUseUnsigned,
                           bool zipDataStreams,
                           const juce::String& className = BinaryBuilder::defaultClassName);

    //==============================================================================
    class ProgressCounter
    {
    public:
        ProgressCounter() noexcept { }
        virtual ~ProgressCounter() noexcept { }

        virtual void processStarted() = 0;
        virtual void processCanceled() = 0;
        virtual void processCompleted() = 0;
        virtual void processFailedToStart() = 0;
        virtual void updateProgress (double progress) = 0;
        virtual bool requestCancellation() = 0;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgressCounter)
    };

    void addProgressCounter (ProgressCounter* counter);

    void removeProgressCounter (ProgressCounter* counter);

    //==============================================================================
    static const juce::String defaultClassName;

private:
    //==============================================================================
    juce::File destinationDirectory;
    bool alwaysUseUnsigned, zipAllDataStreams, shouldCancel;
    int tempNumber;
    double progress;
    juce::Array<juce::File> files;

    juce::ListenerList<ProgressCounter> progressCounters;

    //==============================================================================
    /** @internal */
    void handleAsyncUpdate() override;

    //==============================================================================
    bool anyCountersRequestCancellation();
    void removeNonexistentFiles();
    bool hasValidFiles();
    bool destinationDirectoryExists();
    bool hasValidDestinationDirectory();

    //==============================================================================
    juce::String createValidVersionOfClassName (const juce::String& className) const;
    juce::String temporaryVariableName() const noexcept;
    juce::String externValueType() const noexcept;
    juce::String internalValueType() const noexcept;

    //==============================================================================
    void setupHeader (const juce::String& className,
                      juce::OutputStream& headerStream);

    void setupCPP (const juce::String& className,
                   juce::OutputStream& cppStream);

    //==============================================================================
    enum CreateResult
    {
        CreateCouldntLoad,
        CreateCancelled,
        CreateOkay,
    };

    CreateResult createDataFromFile (const juce::File& file,
                                     const juce::String& className,
                                     juce::OutputStream& headerStream,
                                     juce::OutputStream& cppStream,
                                     bool writeVarSpacing);

    //==============================================================================
    void tryShowInvalidFileList (const juce::Array<juce::File>& invalidFiles);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinaryBuilder);
};

#endif //JRL_BINARY_BUILDER_H

