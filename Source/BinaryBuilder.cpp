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
#include "BinaryBuilder.h"

//==============================================================================
const juce::String BinaryBuilder::defaultClassName = "BinaryData";

//==============================================================================
BinaryBuilder::BinaryBuilder()
{
}

BinaryBuilder::BinaryBuilder (const juce::File& destDir)
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
    {
        files.addIfNotAlreadyThere (file);
    }
}

void BinaryBuilder::addFiles (const juce::Array<juce::File>& newFiles)
{
    for (int i = 0; i < newFiles.size(); ++i)
    {
        addFile (newFiles[i]);
    }
}

void BinaryBuilder::clear()
{
    files.clear();
}

//==============================================================================
void BinaryBuilder::setDestinationDirectory (const juce::File& destDir)
{
    destinationDirectory = destDir;

    jassert (destinationDirectoryExists());
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
                                           "You've not set a class name!\n\nWill use the default one: "
                                         + defaultClassName,
                                           "OK"); //"OK" because I personally dislike "ok" (it just feels wrong...)

        return defaultClassName;
    }

    return validClassName;
}

void BinaryBuilder::removeNonexistentFiles()
{
    juce::StringArray removedFilePaths;

    for (int i = 0; i < files.size(); ++i)
    {
        if (! files[i].existsAsFile())
        {
            removedFilePaths.add (files[i].getFullPathName());
            files.remove (i);
            --i;
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
void BinaryBuilder::setupHeader (const juce::String& className,
                                 juce::OutputStream& headerStream)
{
    headerStream << "#ifndef " << className.toUpperCase() << "_H\r\n"
                    "#define " << className.toUpperCase() << "_H\r\n\r\n"
                    "/**\r\n"
                    "* @file " << className << ".h\r\n"
                    "* @date " << juce::Time::getCurrentTime().toString (true, true, true, true) << "\r\n"
                    "*\r\n"
                    "* Automatically generated binary data\r\n"
                    "*/\r\n"
                    "namespace " << className << "\r\n"
                    "{\r\n";
}

void BinaryBuilder::setupCPP (const juce::String& className,
                              juce::OutputStream& cppStream)
{
    cppStream << "#include \"" << className << ".h\"\r\n\r\n";
}

juce::String BinaryBuilder::temporaryVariableName() noexcept
{
    return "internalTemp";
}

int BinaryBuilder::createDataFromFile (const juce::File& file,
                                       const juce::String& className,
                                       juce::OutputStream& headerStream,
                                       juce::OutputStream& cppStream)
{
    juce::MemoryBlock mb;
    file.loadFileAsData (mb);

    const juce::String name (file.getFileName()
                             .replaceCharacter (' ', '_')
                             .replaceCharacter ('.', '_')
                             .retainCharacters ("abcdefghijklmnopqrstuvwxyz_0123456789"));

    headerStream << "    extern const char*  " << name << ";\r\n"
                    "    const int           " << name << "Size = "
                 << (int) mb.getSize() << ";\r\n\r\n";

    cppStream << "static const unsigned char " << temporaryVariableName() << ++tempNumber << "[] = {";

    size_t i = 0;
    const juce::uint8* const data = (const juce::uint8*) mb.getData();

    while (i < (mb.getSize() - 1))
    {
        if ((i % 40) != 39)
            cppStream << (int) data[i] << ",";
        else
            cppStream << (int) data[i] << ",\r\n ";

        ++i;
    }

    cppStream << (int) data[i] << ",0,0};\r\n";

    cppStream << "const char* " << className << "::" << name
              << " = (const char*)" << temporaryVariableName() << tempNumber << ";\r\n\r\n";

    return mb.getSize();
}

void BinaryBuilder::generateBinaries (const juce::String& className)
{
    const juce::String validClassName (createValidVersionOfClassName (className));

    if (hasValidDestinationDirectory() && hasValidFiles())
    {
        const juce::File headerFile (destinationDirectory.getChildFile (className).withFileExtension (".h"));
        const juce::File cppFile    (destinationDirectory.getChildFile (className).withFileExtension (".cpp"));

        headerFile.deleteFile();
        cppFile.deleteFile();

        juce::ScopedPointer<juce::OutputStream> header (headerFile.createOutputStream());
        juce::ScopedPointer<juce::OutputStream> cpp (cppFile.createOutputStream());

        jassert (header != nullptr); //File is probably in use...
        jassert (cpp != nullptr); //File is probably in use...

        if (header != nullptr && cpp != nullptr)
        {
            setupHeader (className, *header);
            setupCPP (className, *cpp);

            tempNumber = 0;

            for (int i = 0; i < files.size(); ++i)
            {
                createDataFromFile (files[i], className, *header, *cpp);
            }

            *header << "}\r\n\r\n"
                       "#endif //" << className.toUpperCase() << "_H";

            header = cpp = nullptr;
        }
    }
}