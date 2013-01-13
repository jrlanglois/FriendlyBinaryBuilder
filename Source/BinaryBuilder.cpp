#include "BinaryBuilder.h"

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
void BinaryBuilder::generateBinaries (const juce::String& className)
{
    const juce::String validClassName (createValidVersionOfClassName (className));

    if (hasValidFiles())
    {
        if (destinationDirectory == juce::File::nonexistent)
        {
            juce::AlertWindow::showMessageBox (juce::AlertWindow::WarningIcon,
                                               "No destination directory!",
                                               "You must set a destination directory that will be used to contain the generated files!",
                                               "OK");

            return;
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
                        return;
                    }
                }
                break;

                default:
                    return;
                break;
            };
        }
    }
}