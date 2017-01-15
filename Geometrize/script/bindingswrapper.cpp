#include "bindingswrapper.h"

#include <assert.h>

#include <QList>

#include "common/uiactions.h"
#include "formatsupport.h"
#include "searchpaths.h"
#include "util.h"

namespace geometrize
{

namespace script
{

std::string getApplicationDirectoryPath()
{
    return geometrize::searchpaths::getApplicationDirectoryPath();
}

void printToConsole(const std::string& str)
{
    geometrize::util::printToConsole(str);
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
    return geometrize::format::getSupportedImageFileExtensions(false); // TODO?
}

std::vector<std::string> getScriptsForPath(const std::string& dirPath)
{
    return geometrize::util::getScriptsForPath(dirPath);
}

void openJob(const std::string& url)
{
    const QUrl qUrl(QString::fromStdString(url));
    if(!qUrl.isValid()) {
        assert(0 && "Failed to validate url");
        return;
    }

    geometrize::common::ui::openJobs(QStringList(QString::fromStdString(url)));
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

bool stringEndsWith(const std::string& str, const std::string& suffix)
{
    return geometrize::util::stringEndsWith(str, suffix);
}

std::string getAppDataLocation()
{
    return geometrize::util::getAppDataLocation();
}

}

}
