#include "bindingswrapper.h"

#include <assert.h>
#include <QList>

#include "searchpaths.h"
#include "sharedapp.h"
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

std::vector<std::string> getScriptsForPath(const std::string& path)
{
    return geometrize::util::getScriptsForPath(path);
}

void openJob(const std::string& url)
{
    const QUrl qUrl(QString::fromStdString(url));
    if(!qUrl.isValid()) {
        assert(0 && "Failed to validate url");
        return;
    }

    geometrize::app::openJobs(QList<QUrl>{qUrl});
}

}

}
