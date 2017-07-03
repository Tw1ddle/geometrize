#include "streamview.h"

#include <cassert>

#include <QByteArray>
#include <QFile>
#include <QString>

namespace geometrize
{

namespace serialization
{

StreamView::StreamView(const std::string& filePath)
{
    QFile file(QString::fromStdString(filePath));
    if(!file.open(QFile::ReadOnly)) {
        assert(0 && "Failed to read file");
        return;
    }
    m_byteArray = file.readAll();

    char* data = (char*)m_byteArray.data();
    this->setg(data, data, data + m_byteArray.size());
}

}

}
