#pragma once

namespace geometrize
{

namespace logging
{

/**
 * @brief setupLogging Installs a log message handler. Logging in Geometrize uses the Qt QLoggingCategory logging framework
 * See: https://doc.qt.io/qt/qtglobal.html#qInstallMessageHandler
 */
void setupLogging();

}

}
