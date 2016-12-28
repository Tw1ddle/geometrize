#include "util.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

namespace geometrize
{

namespace util
{

void printToConsole(const std::string& str)
{
    qDebug() << QString::fromStdString(str);
}

bool directoryExists(const std::string& dirPath)
{
    return QDir(QString::fromStdString(dirPath)).exists();
}

bool directoryContainsFile(const std::string& dirPath, const std::string& fileName)
{
    const QString qFileName{QString::fromStdString(fileName)};
    QDirIterator it(QString::fromStdString(dirPath));
    while(it.hasNext()) {
        it.next();
        if(QFileInfo(it.filePath()).isFile()) {
            if(it.fileName() == qFileName) {
                return true;
            }
        }
    }
    return false;
}

std::string readFileAsString(const std::string& filePath)
{
    QFile file(QString::fromStdString(filePath));

    if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return file.readAll().toStdString();
    }

    return "";
}

std::vector<std::string> getFilePathsForDirectory(const std::string& dirPath)
{
    std::vector<std::string> files;

    QDirIterator it(QString::fromStdString(dirPath));
    while(it.hasNext()) {
        it.next();
        if(QFileInfo(it.filePath()).isFile()) {
            files.push_back(it.filePath().toStdString());
        }
    }
    return files;
}

inline bool endsWith(std::string const& value, std::string const& ending)
{
    if (ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension)
{
    std::vector<std::string> files{getFilePathsForDirectory(dirPath)};

    files.erase(std::remove_if(files.begin(), files.end(), [&extension](const std::string& s) {
        return !endsWith(s, extension);
    }));

    if(files.empty()) {
        return "";
    }
    return files.front();
}

std::vector<std::string> getScriptsForPath(const std::string& dirPath)
{
    std::vector<std::string> scripts;
    if(!util::directoryExists(dirPath)) {
        return scripts;
    }

    QDirIterator it(QString::fromStdString(dirPath), { "*.chai" }, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        scripts.push_back(it.next().toStdString());
    }

    return scripts;
}

}

}
