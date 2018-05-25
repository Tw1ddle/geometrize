#include "logmessagehandlers.h"

#include <QApplication>
#include <QMessageLogContext>
#include <QString>
#include <QtGlobal>

#include "logger/logmessageevents.h"

namespace geometrize
{

namespace log
{

// TODO?
QEvent* getEvent(const QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    return nullptr;
}

// TODO
void handleLogMessages(const QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    if(type == QtFatalMsg) {
        //abort(); // TODO
        return;
    }

    switch (type) {
    case QtDebugMsg:
        break;
    case QtInfoMsg:
        break;
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        break;
    case QtFatalMsg:
        break;
    }

    // Inspect the message log context to find end points (console widgets etc...)
    // Post the message to the appropriate end points
    //QEvent* event{getEvent(type, context, message)};
    //QApplication::postEvent(QApplication::instance(), event);
}

}

}
