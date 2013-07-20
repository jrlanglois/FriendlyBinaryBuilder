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
#ifndef JRL_BINARY_BUILDER_H
#define JRL_BINARY_BUILDER_H

#include "JuceHeader.h"

/**
    @file BinaryBuilder.h
    @authors Joël R. Langlois, Julian Storer (credited for original code)

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

    /** Destructor */
    ~BinaryBuilder() noexcept;

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
        ~ProgressCounter() noexcept { }

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
    bool alwaysUseUnsigned;
    bool zipAllDataStreams;
    bool shouldCancel;
    int tempNumber;
    double progress;
    juce::Array<juce::File> files;

    juce::ListenerList<ProgressCounter> progressCounters;

    //==============================================================================
    bool anyCountersRequestCancellation();
    
    void handleAsyncUpdate();

    //==============================================================================
    juce::String createValidVersionOfClassName (const juce::String& className) const;

    //==============================================================================
    void removeNonexistentFiles();

    bool hasValidFiles();

    //==============================================================================
    bool destinationDirectoryExists();

    bool hasValidDestinationDirectory();

    //==============================================================================
    juce::String temporaryVariableName() const noexcept;

    juce::String externValueType() const noexcept;

    juce::String internalValueType() const noexcept;

    //==============================================================================
    void setupHeader (const juce::String& className,
                      juce::OutputStream& headerStream);

    void setupCPP (const juce::String& className,
                   juce::OutputStream& cppStream);

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

    void tryShowInvalidFileList (const juce::Array<juce::File>& invalidFiles);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinaryBuilder);
};

#endif //JRL_BINARY_BUILDER_H