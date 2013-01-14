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
* @file BinaryBuilder.h
*
* Simple class used to generate header and cpp files containing binary
* data based off of the added files. The generated files will be placed
* in the set destination directory.
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