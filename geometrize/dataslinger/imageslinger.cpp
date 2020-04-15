#ifdef DATASLINGER_INCLUDED

#include "dataslinger/slinger.h"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <string>

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QList>
#include <QImage>
#include <QPushButton>
#include <QShortcut>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "geometrize/runner/imagerunneroptions.h"

#include "dataslinger/slinger.h"

#include "dialog/imagetaskwindow.h"
#include "dialog/scriptconsole.h"
#include "logger/logmessagehandlers.h"
#include "task/imagetask.h"

namespace
{

void serializeUInt32(std::uint8_t* buf, std::uint32_t val)
{
    buf[0] = static_cast<std::uint8_t>(val);
    buf[1] = static_cast<std::uint8_t>(val >> 8);
    buf[2] = static_cast<std::uint8_t>(val >> 16);
    buf[3] = static_cast<std::uint8_t>(val >> 24);
}

std::int32_t parseUInt32(const std::uint8_t* buf)
{
    // This prevents buf[i] from being promoted to a signed int.
    std::uint32_t u0 = static_cast<std::uint32_t>(buf[0]);
    std::uint32_t u1 = static_cast<std::uint32_t>(buf[1]);
    std::uint32_t u2 = static_cast<std::uint32_t>(buf[2]);
    std::uint32_t u3 = static_cast<std::uint32_t>(buf[3]);

    std::uint32_t uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
    return uval;
}

dataslinger::message::Message makeMessageFromBitmap(const geometrize::Bitmap& bitmap)
{
    // Stored as width, height 4-byte values, followed by byte data
    dataslinger::message::Message data;
    data.resize(8);
    const std::uint32_t width = bitmap.getWidth();
    const std::uint32_t height = bitmap.getHeight();
    serializeUInt32(&data[0], width);
    serializeUInt32(&data[4], height);

    data.insert(data.end(), bitmap.getDataRef().begin(), bitmap.getDataRef().end());

    return data;
}

geometrize::Bitmap makeBitmapFromData(const std::vector<std::uint8_t>& data)
{
    // Read out width and height, followed by byte data
    if(data.size() <= 8) {
        return geometrize::Bitmap(0, 0, std::vector<std::uint8_t>{});
    }

    const std::vector<std::uint8_t> widthAndHeight(data.begin(), data.begin() + 8);

    const std::uint32_t width = parseUInt32(widthAndHeight.data());
    const std::uint32_t height = parseUInt32(widthAndHeight.data() + 4);

    const std::size_t expectedSize = static_cast<std::size_t>(8 + (width * height * 4));
    if(data.size() != expectedSize) {
        return geometrize::Bitmap(0, 0, std::vector<std::uint8_t>{});
    }

    // Assumes RGBA8888 format
    const std::vector<std::uint8_t> imageData(data.begin() + 8, data.end());
    geometrize::Bitmap bitmap(width, height, imageData);
    return bitmap;
}

class SlingerConnectionOptions
{
public:
    template<class Archive>
    void archive(Archive& ar, std::string& host, std::uint16_t& port)
    {
        ar(cereal::make_nvp(hostKey, host));
        ar(cereal::make_nvp(portKey, port));
    }

private:
    const std::string hostKey{"host"};
    const std::string portKey{"port"};
};

class ReceiverConnectionOptions
{
public:
    template<class Archive>
    void archive(Archive& ar, std::string& host, std::uint16_t& port)
    {
        ar(cereal::make_nvp(hostKey, host));
        ar(cereal::make_nvp(portKey, port));
    }

private:
    const std::string hostKey{"host"};
    const std::string portKey{"port"};
};

class ImageSlingerKeyboardShortcutOptions
{
public:
    template<class Archive>
    void archive(Archive& ar, std::string& sendImageShortcut)
    {
        ar(cereal::make_nvp(sendImageKey, sendImageShortcut));
    }

private:
    const std::string sendImageKey{"sendImage"};
};

bool loadSlingerConnectionOptions(const std::string& filePath, std::string& host, std::uint16_t port)
{
    SlingerConnectionOptions options;

    std::ifstream input(filePath.c_str());
    try {
        cereal::JSONInputArchive archive{input};
        options.archive(archive, host, port);
        return true;
    } catch(...) {
        assert(0 && "Failed to read slinger connection options");
    }
    return false;
}

bool loadReceiverConnectionOptions(const std::string& filePath, std::string& host, std::uint16_t port)
{
    ReceiverConnectionOptions options;

    std::ifstream input(filePath.c_str());
    try {
        cereal::JSONInputArchive archive{input};
        options.archive(archive, host, port);
        return true;
    } catch(...) {
        assert(0 && "Failed to read receiver connection options");
    }
    return false;
}

bool loadImageSlingerMiscOptions(const std::string& filePath, std::string& slingImageShortcut)
{
    ImageSlingerKeyboardShortcutOptions options;

    std::ifstream input(filePath.c_str());
    try {
        cereal::JSONInputArchive archive{input};
        options.archive(archive, slingImageShortcut);
        return true;
    } catch(...) {
        assert(0 && "Failed to read image slinger misc options");
    }
    return false;
}

std::string getPreferencesPath(const QString& configFilename)
{
    const QString preferencesFolder{QCoreApplication::applicationDirPath()};
    const QString preferencesPath{QDir(preferencesFolder).filePath(configFilename)};
    const std::string preferencesPathStr{preferencesPath.toStdString()};
    return preferencesPathStr;
}

std::unique_ptr<dataslinger::DataSlinger> imageSender{ nullptr };
std::unique_ptr<dataslinger::DataSlinger> imageReceiver{ nullptr };

std::unique_ptr<dataslinger::DataSlinger> svgShapeSender{ nullptr };

std::int32_t sendImage(const geometrize::Bitmap& bitmap)
{
    static std::int32_t sendAttemptCount = 0;
    sendAttemptCount++;

    if(!imageSender) {
        assert(0 && "imageSender should not be null");
        return sendAttemptCount;
    }

    const dataslinger::message::Message msg = makeMessageFromBitmap(bitmap);
    imageSender->send(msg);

    return sendAttemptCount;
}

}

namespace geometrize
{

void setupImageSlinger()
{
    if(imageSender != nullptr) {
        assert(0 && "Slinger has already been set up");
        return;
    }

    std::string host = "127.0.0.1";
    std::uint16_t port = 8081;
    if(!loadSlingerConnectionOptions(getPreferencesPath("imageslinger_connection_options.json"), host, port)) {
        assert(0 && "Slinger connection preferences failed to load"); // No return since we have some defaults
    }

    imageSender = std::make_unique<dataslinger::DataSlinger>(
    [](const dataslinger::message::Message&) {
        geometrize::log::send("On image sender received message", nullptr);
    },
    [](const dataslinger::event::Event&) {
        geometrize::log::send("On image sender received event", nullptr);
    },
    dataslinger::connection::ConnectionOptions({{
        { dataslinger::connection::ConnectionOption::PREFERRED_BACKEND, dataslinger::connection::PreferredBackend::WEBSOCKET_SERVER },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_HOST_STRING, host },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_PORT_UINT16, port }
    }}));
}

void setupImageReceiver()
{
    if(imageReceiver != nullptr) {
        assert(0 && "imageReceiver has already been set up");
        return;
    }

    std::string host = "127.0.0.2";
    std::uint16_t port = 8082;
    if(!loadReceiverConnectionOptions(getPreferencesPath("imagereceiver_connection_options.json"), host, port)) {
        assert(0 && "imageReceiver connection preferences failed to load"); // No return since we have some defaults
    }

    imageReceiver = std::make_unique<dataslinger::DataSlinger>(
    [](const dataslinger::message::Message&) {
        geometrize::log::send("On image receiver received message", nullptr);
    },
    [](const dataslinger::event::Event&) {
        geometrize::log::send("On image receiver received event", nullptr);
    },
    dataslinger::connection::ConnectionOptions({{
        { dataslinger::connection::ConnectionOption::PREFERRED_BACKEND, dataslinger::connection::PreferredBackend::WEBSOCKET_CLIENT },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_HOST_STRING, host },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_PORT_UINT16, port }
    }}));
}

void installImageSlingerUserInterface(geometrize::dialog::ImageTaskWindow* widget)
{
    // Create widget containing slinger user interface
    // And attach it to the bottom of the image task window in a dock

    QDockWidget* slingerDockWidget = new QDockWidget("Dataslinger Settings", widget);
    slingerDockWidget->setFeatures(slingerDockWidget->features() & ~QDockWidget::DockWidgetFeature::DockWidgetClosable);
    QWidget* slingerDockContents = new QWidget(slingerDockWidget);
    slingerDockWidget->setWidget(slingerDockContents);

    QHBoxLayout* slingerWidgetLayout = new QHBoxLayout();
    QVBoxLayout* slingerOptionsLayout = new QVBoxLayout();

    QPushButton* imageSendButton = new QPushButton(slingerDockContents);
    imageSendButton->setText("Send Image");

    QLabel* imageSendCountLabel = new QLabel(slingerDockContents);
    imageSendCountLabel->setText("Image Send Attempts: -");

    geometrize::dialog::ScriptConsole* slingerConsole = new geometrize::dialog::ScriptConsole(slingerDockContents);

    slingerOptionsLayout->addWidget(imageSendButton);
    slingerOptionsLayout->addWidget(imageSendCountLabel);

    slingerWidgetLayout->addItem(slingerOptionsLayout);
    slingerWidgetLayout->addWidget(slingerConsole);

    slingerDockContents->setLayout(slingerWidgetLayout);
    widget->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, slingerDockWidget);

    // Helper function to send out an image, logging errors and stats
    const auto sendImageHelper = [widget, slingerConsole, imageSendCountLabel]() {
        const auto task = widget->getImageTask();
        if(task == nullptr) {
            geometrize::log::send("Dataslinger - will fail to send image data, no image task is set on the widget", slingerConsole);
            return;
        }

        const std::int32_t sendAttemptCount = ::sendImage(task->getCurrent());

        if(imageSender == nullptr) {
            geometrize::log::send("Dataslinger - will fail to send image, data slinger was not set up", slingerConsole);
        } else {
            geometrize::log::send("Dataslinger - sending image (" + QString::number(sendAttemptCount) + " image(s) this session)", slingerConsole);
        }

        imageSendCountLabel->setText("Image Send Attempts: " + QString::number(sendAttemptCount));
    };

    // Connect up send image push button
    QObject::connect(imageSendButton, &QPushButton::pressed, [sendImageHelper]() {
        sendImageHelper();
    });

    // Install keyboard shortcuts
    std::string slingImageKeyboardShortcut = "S";
    if(!loadImageSlingerMiscOptions(getPreferencesPath("imageslinger_misc_options.json"), slingImageKeyboardShortcut)) {
        assert(0 && "Slinger misc preferences failed to load"); // No return since we have some defaults
    }
    QShortcut* sendShortcut = new QShortcut(QKeySequence(QString::fromStdString(slingImageKeyboardShortcut)), widget);
    sendShortcut->setContext(Qt::ApplicationShortcut);
    QObject::connect(sendShortcut, &QShortcut::activated, [sendImageHelper]() {
        sendImageHelper();
    });

    // Set up polling so the image sender can receive events itself and update the console
    QTimer* senderPollTimer = new QTimer(widget);
    senderPollTimer->setInterval(100);
    QObject::connect(senderPollTimer, &QTimer::timeout, [slingerConsole]() {
        if(imageSender == nullptr) {
            geometrize::log::send("Dataslinger - failed to poll image sender, data sligner was not set up", slingerConsole);
        }
        imageSender->poll();
    });
    senderPollTimer->start();
}

void setupSvgShapeSlinger()
{
    if(svgShapeSender != nullptr) {
        assert(0 && "Slinger has already been set up");
        return;
    }

    std::string host = "127.0.0.1";
    std::uint16_t port = 9001;
    if(!loadSlingerConnectionOptions(getPreferencesPath("svgshapeslinger_connection_options.json"), host, port)) {
        assert(0 && "Slinger connection preferences failed to load"); // No return since we have some defaults
    }

    svgShapeSender = std::make_unique<dataslinger::DataSlinger>(
    [](const dataslinger::message::Message&) {
        geometrize::log::send("On SVG shape sender received message", nullptr);
    },
    [](const dataslinger::event::Event&) {
        geometrize::log::send("On SVG shape sender received event", nullptr);
    },
    dataslinger::connection::ConnectionOptions({{
        { dataslinger::connection::ConnectionOption::PREFERRED_BACKEND, dataslinger::connection::PreferredBackend::WEBSOCKET_SERVER },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_HOST_STRING, host },
        { dataslinger::connection::ConnectionOption::WEBSOCKET_PORT_UINT16, port }
    }}));
}

void sendSvgShapeData(const std::string& s)
{
    if(svgShapeSender == nullptr) {
        assert(0 && "SVG shape sender isn't set up");
        return;
    }

    const dataslinger::message::Message msg(s.begin(), s.end());
    svgShapeSender->send(msg);
}

}

#endif
