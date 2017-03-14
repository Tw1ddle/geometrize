#pragma once

#include <QString>

class QWidget;

namespace geometrize
{

namespace log
{

/**
 * @brief send Sends a message to the target widget. The target will receive a TextualWidgetMessageEvent.
 * @param message The message to send.
 * @param target The target widget.
 */
void send(const QString& message, QWidget* target);

}

}
