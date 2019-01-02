#include "util.h"

#include <cassert>
#include <csignal>
#include <fstream>
#include <random>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <QUrl>

#include "geometrize/commonutil.h"

#ifdef _MSC_VER
#include <intrin.h> // For debugbreak
#endif

namespace geometrize
{

namespace util
{

void debugBreak()
{
#ifdef _MSC_VER
    __debugbreak();
#elif __MINGW32__
    // Not sure how to break here...
    assert(0 && "Mingw32 debug break");
#else
    raise(SIGTRAP);
#endif
}

void printToConsole(const std::string& str)
{
    qDebug() << QString::fromStdString(str);
}

void messageBox(const std::string& str)
{
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();
}

bool fileExists(const std::string& filePath)
{
    return QFile(QString::fromStdString(filePath)).exists();
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
        return QString(file.readAll()).toStdString();
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

std::vector<std::string> getSubdirectoriesForDirectory(const std::string& dirPath)
{
    std::vector<std::string> files;

    QDirIterator it(QString::fromStdString(dirPath));
    while(it.hasNext()) {
        it.next();
        if(QFileInfo(it.filePath()).isDir()) {
            files.push_back(it.filePath().toStdString());
        }
    }
    return files;
}

inline bool endsWith(const std::string& value, const std::string& ending)
{
    if (value.length() >= ending.length()) {
        return (0 == value.compare (value.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::vector<std::string> getFilesWithExtension(const std::string& dirPath, const std::string& extension)
{
    std::vector<std::string> files{getFilePathsForDirectory(dirPath)};

    files.erase(std::remove_if(files.begin(), files.end(), [&extension](const std::string& s) {
        return !endsWith(s, extension);
    }), files.end());

    return files;
}

std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension)
{
    const std::vector<std::string> files{getFilesWithExtension(dirPath, extension)};

    if(files.empty()) {
        return "";
    }
    return files.front();
}

std::string getFirstFileWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions)
{
    for(const std::string& ext : extensions) {
        const std::string s{getFirstFileWithExtension(dirPath, ext)};
        if(!s.empty()) {
            return s;
        }
    }

    return "";
}

std::vector<std::string> getScriptsForPath(const std::string& dirPath)
{
    std::vector<std::string> scripts;
    if(!util::directoryExists(dirPath)) {
        assert(0 && "Script directory does not exist");
        return scripts;
    }

    QDirIterator it(QString::fromStdString(dirPath), { "*.chai" }, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        scripts.push_back(it.next().toStdString());
    }

    return scripts;
}

std::string getDirectoryForFilePath(const std::string& filePath)
{
    const QFileInfo fileInfo{QString::fromStdString(filePath)};

    if(!fileInfo.exists()) {
        assert(0 && "File does not exist");
    }

    return fileInfo.absoluteDir().absolutePath().toStdString();
}

bool openInDefaultApplication(const std::string& path)
{
    return QDesktopServices::openUrl(QUrl::fromUserInput(QString::fromStdString(path)));
}

bool revealInDefaultApplication(const std::string& path)
{
    const QUrl directoryPath{QUrl::fromUserInput(QString::fromStdString(path)).adjusted(QUrl::RemoveFilename)};
    return QDesktopServices::openUrl(directoryPath);
}

void clearGlobalClipboard()
{
    QApplication::clipboard()->clear();
}

std::string getGlobalClipboardText()
{
    return QApplication::clipboard()->text().toStdString();
}

void setGlobalClipboardText(const std::string& text)
{
    QApplication::clipboard()->setText(QString::fromStdString(text));
}

bool stringBeginsWith(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool stringEndsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string getAppDataLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation).toStdString();
}

bool writeStringToFile(const std::string& str, const std::string& path)
{
    std::ofstream out(path, std::ios::trunc | std::ios::out);
    if(!out.is_open()) {
        return false;
    }

    if(out) {
        out << str;
    }

    out.close();

    if(!out) {
        return false;
    }

    return true;
}

std::string percentEncode(const std::string& str)
{
    const QString s{QString::fromStdString(str)};
    const QString percentEncoded{QUrl::toPercentEncoding(s)};
    return percentEncoded.toStdString();
}

int randomInRange(const int lower, const int upper)
{
    assert(lower <= upper);
    return geometrize::commonutil::randomRange(lower, upper);
}

int clamp(const int value, const int lower, const int upper)
{
    assert(lower <= upper);
    return std::max(lower, std::min(value, upper));
}

std::vector<std::string> split(const std::string& s, const char delimiter)
{
    std::vector<std::string> elements;
    split(s, delimiter, std::back_inserter(elements));
    return elements;
}

}

}
