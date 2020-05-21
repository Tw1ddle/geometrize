#pragma once

#include <QObject>

class QCoreApplication;
class QEvent;

namespace geometrize
{

/**
 * @brief The TabletProximityEventFilter captures tablet enter/exit proximity events (like when Wacom pens are held close to the screen)
 * Used to hide the cursor so it doesn't get in the way of wherever the pen is being held
 */
class TabletProximityEventFilter : public QObject
{
    Q_OBJECT
public:
    TabletProximityEventFilter(QCoreApplication* app);

    bool eventFilter(QObject* /*obj*/, QEvent* event) override;

signals:
    /**
     * @brief signal_onTabletEnterProximity Signal dispatched when a Wacom tablet enter proximity event is received
     */
    void signal_onTabletEnterProximity();

    /**
     * @brief signal_onTabletLeaveProximity Signal dispatched when a Wacom tablet leave proximity event is received
     */
    void signal_onTabletLeaveProximity();
};

/**
 * @brief getSharedTabletProximityEventFilterInstance Gets a pointer to the shared instance of the tablet proximity event filter (since we only need one at the top level)
 * @return A reference to the tablet proximity event filter
 */
TabletProximityEventFilter* getSharedTabletProximityEventFilterInstance();

}
