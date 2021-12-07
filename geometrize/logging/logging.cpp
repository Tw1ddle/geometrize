#include "logging/logging.h"

#include <string>

#include <QByteArray>
#include <QtGlobal>
#include <QLoggingCategory>
#include <QMessageLogContext>
#include <QString>

#include "geometrize/common/util.h"

namespace
{

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    const QByteArray localMsg = msg.toLocal8Bit();
    geometrize::util::printToAllScriptConsoleWidgets("Log message: " + localMsg.toStdString());
}

}

namespace geometrize
{

namespace logging
{

void setupLogging()
{
    qInstallMessageHandler(&messageHandler);
}

}

}
