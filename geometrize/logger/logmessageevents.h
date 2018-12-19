#pragma once

#include <QEvent>
#include <QString>

class QWidget;

namespace geometrize
{

namespace log
{

/**
 * @brief The LogMessageEvent class is the base class of log message events.
 * This sort of event should be posted to the QApplication when a log message is handled by the custom message handler.
 * This sort of event represents the intended end-point of the log message event.
 */
class LogMessageEvent : public QEvent
{
public:
    LogMessageEvent(QEvent::Type type, QString message);

    static const QEvent::Type textualWidgetEventType;

    QString getMessage() const;

private:
    QString m_message;
};

/**
 * @brief The TextualWidgetMessageEvent class is a log message directed at a Qt console window (or some textual widget).
 */
class TextualWidgetMessageEvent : public LogMessageEvent
{
public:
    /**
     * @brief TextualWidgetMessageEvent Creates the textual widget message event.
     * @param message The message to bundle into the event.
     */
    TextualWidgetMessageEvent(QString message);
};

/**
 * @brief send Sends a message to the target widget. The target will receive a TextualWidgetMessageEvent.
 * @param message The message to send.
 * @param target The target widget.
 */
void send(const QString& message, QWidget* target);

}

}
