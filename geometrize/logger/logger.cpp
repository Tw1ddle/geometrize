#include "logger.h"

#include <QApplication>
#include <QString>
#include <QWidget>

#include "logger/logmessageevents.h"

namespace geometrize
{

namespace log
{

void send(const QString& message, QWidget* target)
{
    QApplication::postEvent(target, new geometrize::log::TextualWidgetMessageEvent(message));
}

}

}
