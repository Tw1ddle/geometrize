#include "tabletproximityeventfilter.h"

#include <QApplication>
#include <QEvent>
#include <QObject>

namespace geometrize
{

TabletProximityEventFilter::TabletProximityEventFilter(QCoreApplication* app) : QObject(app)
{
    // Default behaviours - show/hide cursor when the tablet pointer enters/leaves proximity
    connect(this, &TabletProximityEventFilter::signal_onTabletEnterProximity, []() {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    });
    connect(this, &TabletProximityEventFilter::signal_onTabletLeaveProximity, []() {
        QApplication::restoreOverrideCursor();
    });
}

bool TabletProximityEventFilter::eventFilter(QObject* /*obj*/, QEvent* event)
{
    if (event->type() == QEvent::TabletEnterProximity) {
        signal_onTabletEnterProximity();
    }
    if(event->type() == QEvent::TabletLeaveProximity) {
        signal_onTabletLeaveProximity();
    }
    return false;
}

TabletProximityEventFilter& getSharedTabletProximityEventFilterInstance()
{
    static TabletProximityEventFilter* sharedInstance = new TabletProximityEventFilter(QApplication::instance());
    return *sharedInstance;
}

}
