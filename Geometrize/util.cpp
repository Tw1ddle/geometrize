#include "util.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

namespace geometrize
{

namespace util
{

bool directoryExists(const QString& dirPath)
{
    return QDir(dirPath).exists();
}

bool directoryContainsFile(const QString& dirPath, const QString& fileName)
{
    QDirIterator it(dirPath);
    while(it.hasNext()) {
        it.next();
        if(QFileInfo(it.filePath()).isFile()) {
            if(it.fileName() == fileName) {
                return true;
            }
        }
    }
    return false;
}

std::string readFileAsString(const QString& filePath)
{
    QFile file(filePath);

    if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.readAll().toStdString();
    }

    return "";
}

}

}
