#ifdef DATASLINGER_INCLUDED

#include "dataslinger/slinger.h"

#include <cstdint>
#include <memory>
#include <string>

#include <QImage>

#include "dataslinger/connection/connectionoptions.h"
#include "dataslinger/event/event.h"
#include "dataslinger/message/message.h"

namespace
{

void serializeInt32(std::uint8_t* buf, std::int32_t val)
{
    std::uint32_t uval = static_cast<std::uint32_t>(val);
    buf[0] = static_cast<std::uint8_t>(uval);
    buf[1] = static_cast<std::uint8_t>(uval >> 8);
    buf[2] = static_cast<std::uint8_t>(uval >> 16);
    buf[3] = static_cast<std::uint8_t>(uval >> 24);
}

std::int32_t parseInt32(const std::uint8_t* buf)
{
    // This prevents buf[i] from being promoted to a signed int.
    std::uint32_t u0 = static_cast<std::uint32_t>(buf[0]);
    std::uint32_t u1 = static_cast<std::uint32_t>(buf[1]);
    std::uint32_t u2 = static_cast<std::uint32_t>(buf[2]);
    std::uint32_t u3 = static_cast<std::uint32_t>(buf[3]);

    std::uint32_t uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
    return static_cast<std::int32_t>(uval);
}

dataslinger::message::Message makeMessageFromImage(const QImage& image)
{
    // Stored as width, height 4-byte values, followed by byte data
    dataslinger::message::Message data;
    data.resize(8);
    const std::int32_t width = image.width();
    const std::int32_t height = image.height();
    serializeInt32(&data[0], width);
    serializeInt32(&data[4], height);

    for(std::size_t i = 0 ; i < static_cast<std::size_t>(image.sizeInBytes()); i++) {
        data.push_back(static_cast<std::uint8_t>(*(image.bits() + i)));
    }

    return data;
}

QImage makeImageFromData(const std::vector<std::uint8_t>& data)
{
    // Read out width and height, followed by byte data
    if(data.size() <= 8) {
        return QImage();
    }

    const std::vector<std::uint8_t> widthAndHeight(data.begin(), data.begin() + 8);

    const std::int32_t width = parseInt32(widthAndHeight.data());
    const std::int32_t height = parseInt32(widthAndHeight.data() + 4);

    const std::size_t expectedSize = static_cast<std::size_t>(8 + (width * height * 4));
    if(data.size() != expectedSize) {
        return QImage();
    }

    // Assumes RGBA8888 format
    QImage image(reinterpret_cast<const uchar*>(data.data() + 8), static_cast<int>(width), static_cast<int>(height), QImage::Format_RGBA8888);
    return image;
}

}

namespace geometrize
{

void setupImageSlinger()
{
    // TODO
    const std::uint16_t port = 0;
    const std::string backend = "";
    const std::string host = "";

    auto connectionOptions = dataslinger::connection::ConnectionOptions({{{
        { dataslinger::connection::ConnectionOption::PREFERRED_BACKEND, backend },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_HOST_STRING, host },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_PORT_UINT16, port }
    }}});
}

}

#endif
