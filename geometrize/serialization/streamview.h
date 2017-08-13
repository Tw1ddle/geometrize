#pragma once

#include <iosfwd>
#include <streambuf>
#include <string>

#include <QByteArray>

namespace geometrize
{

namespace serialization
{

/**
 * @brief The StreamView class defines a custom stream buffer view that allows Cereal to work with Qt resources packed into executables.
 * This is necessary because Cereal uses istreams directly, but we need to work with Qt resources.
 */
class StreamView : public std::streambuf
{
public:
    StreamView(const std::string& filePath);

private:
    QByteArray m_byteArray;
};

}

}
