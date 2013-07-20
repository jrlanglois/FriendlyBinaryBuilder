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
#include "BinaryBuilder.h"

//==============================================================================
const juce::String BinaryBuilder::defaultClassName = "BinaryData";

//==============================================================================
BinaryBuilder::BinaryBuilder() :
    alwaysUseUnsigned (false),
    zipAllDataStreams (false)
{
}

BinaryBuilder::BinaryBuilder (const juce::File& destDir) :
    alwaysUseUnsigned (false),
    zipAllDataStreams (false)
{
    setDestinationDirectory (destDir);
}

BinaryBuilder::~BinaryBuilder() noexcept
{
}

//==============================================================================
void BinaryBuilder::addFile (const juce::File& file)
{
    jassert (! file.isDirectory());
    jassert (file.existsAsFile());

    if (file.existsAsFile())
        files.addIfNotAlreadyThere (file);
}

void BinaryBuilder::addFiles (const juce::Array<juce::File>& newFiles)
{
    for (int i = 0; i < newFiles.size(); ++i)
        addFile (newFiles.getReference (i));
}

void BinaryBuilder::clear()
{
    files.clear();
}

//==============================================================================
void BinaryBuilder::setDestinationDirectory (const juce::File& destDir)
{
    destinationDirectory = destDir;
}

bool BinaryBuilder::destinationDirectoryExists()
{
    jassert (! destinationDirectory.existsAsFile()); //The path is pointing to a file!

    return destinationDirectory.exists();
}

//==============================================================================
juce::String BinaryBuilder::createValidVersionOfClassName (const juce::String& className) const
{
    const juce::String validClassName (className.trim());

    if (validClassName.isEmpty())
    {
        juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                           "No class name!",
                                           "You've not set a class name!\n\nWill use the default one: " + defaultClassName,
                                           "OK");

        return defaultClassName;
    }

    return validClassName;
}

void BinaryBuilder::removeNonexistentFiles()
{
    juce::StringArray removedFilePaths;

    for (int i = files.size(); --i >= 0;)
    {
        if (! files.getReference (i).existsAsFile())
        {
            removedFilePaths.add (files.getReference (i).getFullPathName());
            files.remove (i);
        }
    }

    if (removedFilePaths.size() > 0)
    {
        juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                           "Removed the following nonexistant files:",
                                           removedFilePaths.joinIntoString ("\n"),
                                           "OK");
    }
}

bool BinaryBuilder::hasValidFiles()
{
    removeNonexistentFiles();

    if (files.size() <= 0)
    {
        juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                           "No files!",
                                           "There aren't any files to create binaries from!",
                                           "OK");

        return false;
    }

    return true;
}

//==============================================================================
bool BinaryBuilder::hasValidDestinationDirectory()
{
    if (destinationDirectory == juce::File::nonexistent)
    {
        juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                           "No destination directory!",
                                           "You must set a destination directory that will be used to contain the generated files!",
                                           "OK");

        return false;
    }

    if (! destinationDirectoryExists())
    {
        juce::AlertWindow destDir ("Destination folder does not exist!",
                                   "Would you like to attempt creating the missing folder(s) in the path?",
                                   juce::AlertWindow::InfoIcon);

        destDir.addButton ("Yes", 0, juce::KeyPress (juce::KeyPress::returnKey));
        destDir.addButton ("No", 1, juce::KeyPress (juce::KeyPress::escapeKey));

        switch (destDir.runModalLoop())
        {
            case 0:
            {
                if (destinationDirectory.createDirectory().failed())
                {
                    juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                                       "Failed creating the directory!",
                                                       "The path may be invalid or inaccessible. Please select a different folder.",
                                                       "OK");

                    return false;
                }
            }
            break;

            default:
                return false;
            break;
        };
    }

    return true;
}

//==============================================================================
juce::String BinaryBuilder::temporaryVariableName() const noexcept
{
    return "internalTemp";
}

juce::String BinaryBuilder::externValueType() const noexcept
{
    if (alwaysUseUnsigned)
        return "const unsigned char*";

    return "const char*"; 
}

juce::String BinaryBuilder::internalValueType() const noexcept
{
    if (alwaysUseUnsigned)
        return "const unsigned char*";

    return "const char*"; 
}

//==============================================================================
void BinaryBuilder::setupHeader (const juce::String& className, juce::OutputStream& headerStream)
{
    headerStream << "#ifndef " << className.toUpperCase() << "_H\r\n"
                    "#define " << className.toUpperCase() << "_H\r\n\r\n"
                    "/**\r\n"
                    "    @file " << className << ".h\r\n"
                    "    @date " << juce::Time::getCurrentTime().toString (true, true, true, true) << "\r\n"
                    "    \r\n"
                    "    Automatically generated binary data\r\n"
                    "*/\r\n"
                    "namespace " << className << "\r\n"
                    "{\r\n";
}

void BinaryBuilder::setupCPP (const juce::String& className, juce::OutputStream& cppStream)
{
    cppStream << "#include \"" << className << ".h\"\r\n\r\n";
}

//==============================================================================
BinaryBuilder::CreateResult BinaryBuilder::createDataFromFile (const juce::File& file,
                                                               const juce::String& className,
                                                               juce::OutputStream& headerStream,
                                                               juce::OutputStream& cppStream,
                                                               const bool writeVarSpacing)
{
    juce::MemoryBlock mb;

    if (! file.loadFileAsData (mb))
        return CreateCouldntLoad;

    if (zipAllDataStreams)
    {
        juce::MemoryOutputStream memStream;
        juce::GZIPCompressorOutputStream zipper (&memStream, 9);
        zipper.write (mb.getData(), mb.getSize());
        zipper.flush();
        mb = memStream.getMemoryBlock();
    }

    const size_t size = mb.getSize();

    const juce::String tempVarName (temporaryVariableName() + juce::String (++tempNumber));
    const juce::String chars ("abcdefghijklmnopqrstuvwxyz");
    const juce::String name (file.getFileName()
                             .replaceCharacter (' ', '_')
                             .replaceCharacter ('.', '_')
                             .retainCharacters ("_0123456789" + chars.toLowerCase() + chars.toUpperCase()));

    //Write to the Header file:
    headerStream << "    extern " << externValueType() << " " << name << ";\r\n";
    headerStream << "    const int          " << name << "Size = " << juce::String (size) << ";\r\n";

    if (writeVarSpacing)
        headerStream << "\r\n";

    //Write to the CPP file:
    cppStream << "static " << internalValueType() << " " << tempVarName << "[] = \r\n{\r\n    ";

    const juce::uint8* const data = (const juce::uint8*) mb.getData();

    size_t i = 0;
    while (i < (size - 1))
    {
        cppStream << (int) data[i] << ",";

        if ((i % 30) == 29)
            cppStream << "\r\n    ";

        ++i;

        if (anyCountersRequestCancellation())
            return CreateCancelled;
    }

    cppStream << (int) data[i] << ",0,0" << "\r\n};\r\n\r\n";

    cppStream << internalValueType() << " " << className << "::" << name;
    cppStream << " = " << tempVarName << ";";

    if (writeVarSpacing)
        cppStream << "\r\n\r\n//==============================================================================";

    return CreateOkay;
}

//==============================================================================
void BinaryBuilder::generateBinaries (const bool useUnsigned, const bool zipDataStreams,
                                      const juce::String& className)
{
    const juce::String validClassName (createValidVersionOfClassName (className));

    if (hasValidDestinationDirectory() && hasValidFiles())
    {
        alwaysUseUnsigned = useUnsigned;
        zipAllDataStreams = zipDataStreams;

        const juce::File headerFile (destinationDirectory.getChildFile (className).withFileExtension (".h"));
        const juce::File cppFile (destinationDirectory.getChildFile (className).withFileExtension (".cpp"));

        headerFile.deleteFile();
        cppFile.deleteFile();

        juce::ScopedPointer<juce::OutputStream> header (headerFile.createOutputStream());
        juce::ScopedPointer<juce::OutputStream> cpp (cppFile.createOutputStream());

        if (header != nullptr && cpp != nullptr)
        {
            setupHeader (className, *header);
            setupCPP (className, *cpp);

            tempNumber = 0;

            const int numFiles = files.size();

            juce::Array<juce::File> invalidFiles;

            bool deleteFiles = false;

            for (int i = 0; i < numFiles; ++i)
            {
                const juce::File& fileToProcess = files.getReference (i);
                const CreateResult result =  createDataFromFile (fileToProcess, className,
                                                                 *header, *cpp, i != (numFiles - 1));

                switch (result)
                {
                    case CreateCouldntLoad:
                        invalidFiles.addIfNotAlreadyThere (fileToProcess);
                    break;

                    case CreateCancelled:
                        deleteFiles = true;
                    break;

                    default: break;
                };

                if (deleteFiles)
                    break;
            }

            *header << "}\r\n\r\n" << "#endif //" << className.toUpperCase() << "_H";
            header = cpp = nullptr;

            if (deleteFiles)
            {
                headerFile.deleteFile();
                cppFile.deleteFile();
                progressCounters.call (&ProgressCounter::processCanceled);
            }
            else
            {
                tryShowInvalidFileList (invalidFiles);
                progressCounters.call (&ProgressCounter::processCompleted);
            }
        }
        else
        {
            juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                               "The destination files are currently in use!",
                                               "You must stop using them in order to leave FriendlyBinaryBuilder overwrite them.",
                                               "OK");

            progressCounters.call (&ProgressCounter::processFailedToStart);
        }
    }
}

void BinaryBuilder::tryShowInvalidFileList (const juce::Array<juce::File>& invalidFiles)
{
    if (invalidFiles.size() > 0)
    {
        juce::String fileList ("The files listed below were not added.");
        fileList += juce::newLine + "It's possible they were too large to write as binary.";

        for (int i = 0; i < invalidFiles.size(); ++i)
            fileList += invalidFiles.getReference (i).getFullPathName() + juce::newLine;

        juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                           "Some files weren't added!",
                                           fileList,
                                           "OK");
    }
}

//==============================================================================
void BinaryBuilder::addProgressCounter (ProgressCounter* counter)
{
    progressCounters.add (counter);
}

void BinaryBuilder::removeProgressCounter (ProgressCounter* counter)
{
    progressCounters.remove (counter);
}

bool BinaryBuilder::anyCountersRequestCancellation()
{
    const juce::Array<ProgressCounter*>& list = progressCounters.getListeners();

    for (int i = list.size(); --i >= 0;)
        if (list.getUnchecked (i)->requestCancellation())
            return true;

    return false;
}
    
void BinaryBuilder::handleAsyncUpdate()
{
    progressCounters.call (&ProgressCounter::updateProgress, progress);
}