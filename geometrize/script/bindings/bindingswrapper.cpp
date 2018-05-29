#include "bindingswrapper.h"

#include <cassert>

#include <QString>

#include "common/formatsupport.h"
#include "common/searchpaths.h"
#include "common/util.h"
#include "localization/localization.h"
#include "task/taskutil.h"

namespace geometrize
{

namespace script
{

namespace bindings
{

std::string getApplicationDirectoryPath()
{
    return geometrize::searchpaths::getApplicationDirectoryPath();
}

void printToConsole(const std::string& str)
{
    geometrize::util::printToConsole(str);
}

void messageBox(const std::string& str)
{
    geometrize::util::messageBox(str);
}

void debugBreak()
{
    geometrize::util::debugBreak();
}

bool fileExists(const std::string& filePath)
{
    return geometrize::util::fileExists(filePath);
}

bool directoryExists(const std::string& dirPath)
{
    return geometrize::util::directoryExists(dirPath);
}

bool directoryContainsFile(const std::string& dirPath, const std::string& fileName)
{
    return geometrize::util::directoryContainsFile(dirPath, fileName);
}

std::string readFileAsString(const std::string& filePath)
{
    return geometrize::util::readFileAsString(filePath);
}

std::vector<std::string> getFilePathsForDirectory(const std::string& dirPath)
{
    return geometrize::util::getFilePathsForDirectory(dirPath);
}

std::vector<std::string> getScriptSearchPaths()
{
    return geometrize::searchpaths::getScriptSearchPaths();
}

std::vector<std::string> getTemplateSearchPaths()
{
    return geometrize::searchpaths::getTemplateSearchPaths();
}

std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension)
{
    return geometrize::util::getFirstFileWithExtension(dirPath, extension);
}

std::string getFirstFileWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions)
{
    return geometrize::util::getFirstFileWithExtensions(dirPath, extensions);
}

std::vector<std::string> getFilesWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions)
{
    std::vector<std::string> files;

    for(const std::string& extension : extensions) {
        // TODO
    }

    return files;
}

std::vector<std::string> getFilesWithExtension(const std::string& dirPath, const std::string& extension)
{
    return getFilesWithExtensions(dirPath, { extension });
}

std::vector<std::string> getSupportedImageFileExtensions()
{
    return geometrize::format::getReadableImageFileExtensions(false);
}

std::vector<std::string> getScriptsForPath(const std::string& dirPath)
{
    return geometrize::util::getScriptsForPath(dirPath);
}

void openTask(const std::string& url, const bool addToRecents)
{
    // Horrible workaround because QUrl doesn't handle Qt resource file prefixes well
    // See: https://forum.qt.io/topic/1494/universal-solution-for-resource-prefix
    // Note this breaks QUrl validation too (QUrl isValid chokes when given this modified url)
    QString qUrl{QString::fromStdString(url)};
    const QString strToReplace{":/"};
    if(qUrl.startsWith(strToReplace)) {
        qUrl.replace(0, strToReplace.size(), "qrc:///");
    }

    geometrize::util::openTasks(QStringList(QString::fromStdString(url)), addToRecents);
}

bool openInDefaultApplication(const std::string& path)
{
    return geometrize::util::openInDefaultApplication(path);
}

bool revealInDefaultApplication(const std::string& path)
{
    return geometrize::util::revealInDefaultApplication(path);
}

void clearGlobalClipboard()
{
    geometrize::util::clearGlobalClipboard();
}

std::string getGlobalClipboardText()
{
    return geometrize::util::getGlobalClipboardText();
}

void setGlobalClipboardText(const std::string& text)
{
    geometrize::util::setGlobalClipboardText(text);
}

bool stringBeginsWith(const std::string& str, const std::string& prefix)
{
    return geometrize::util::stringBeginsWith(str, prefix);
}

bool stringEndsWith(const std::string& str, const std::string& suffix)
{
    return geometrize::util::stringEndsWith(str, suffix);
}

std::string getAppDataLocation()
{
    return geometrize::util::getAppDataLocation();
}

bool writeStringToFile(const std::string& str, const std::string& path)
{
    return geometrize::util::writeStringToFile(str, path);
}

std::string percentEncode(const std::string& str)
{
    return geometrize::util::percentEncode(str);
}

int randomInRange(const int lower, const int upper)
{
    return geometrize::util::randomInRange(lower, upper);
}

int clamp(const int value, const int lower, const int upper)
{
    return geometrize::util::clamp(value, lower, upper);
}

std::vector<std::string> split(const std::string& s, const char delimiter)
{
    return geometrize::util::split(s, delimiter);
}

void setTranslatorsForLocale(const std::string& locale)
{
    geometrize::setTranslatorsForLocale(QString::fromStdString(locale));
}

}

}

}
