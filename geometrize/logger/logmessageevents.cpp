#include "logmessageevents.h"

#include <QApplication>
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

void send(const QString& message, QWidget* target)
{
    QApplication::postEvent(target, new geometrize::log::TextualWidgetMessageEvent(message));
}

}

}
