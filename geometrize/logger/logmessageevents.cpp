#include "logmessageevents.h"

#include <QEvent>
#include <QString>
#include <QWidget>

namespace geometrize
{

namespace log
{

LogMessageEvent::LogMessageEvent(const QEvent::Type type, const QString message) : QEvent{type}, m_message{message} {}

QString LogMessageEvent::getMessage() const
{
    return m_message;
}

TextualWidgetMessageEvent::TextualWidgetMessageEvent(const QString message) : LogMessageEvent(LogMessageEvent::textualWidgetEventType, message) {}

const QEvent::Type LogMessageEvent::textualWidgetEventType = static_cast<QEvent::Type>(QEvent::registerEventType());

}

}
