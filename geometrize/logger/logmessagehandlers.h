#pragma once

#include <QMessageLogContext>
#include <QString>
#include <QtGlobal>

namespace geometrize
{

namespace log
{

/**
 * @brief handleLogMessages A Qt message handler function.
 * @param type The type of message received by the message handler.
 * @param context Additional information about the log message.
 * @param message The log message itself.
 */
void handleLogMessages(QtMsgType type, const QMessageLogContext& context, const QString& message);

}

}
