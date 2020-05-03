#include "customtabletevent.h"

#include <QGraphicsSceneEvent>
#include <QWidget>

namespace geometrize
{

namespace scene
{

QString tabletEventTypeToString(const TabletEventType& type)
{
    switch(type) {
    case TabletEventType::Move:
        return "Move";
    case TabletEventType::Press:
        return "Press";
    case TabletEventType::Release:
        return "Release";
    case TabletEventType::EnterProximity:
        return "Enter Proximity";
    case TabletEventType::LeaveProximity:
        return "Leave Proximity";
    default:
        return "Unknown";
    }
}

QString tabletEventPointerTypeToString(const TabletEventPointerType& type)
{
    switch(type) {
    case TabletEventPointerType::Pen:
        return "Pen";
    case TabletEventPointerType::Cursor:
        return "Cursor";
    case TabletEventPointerType::Eraser:
        return "Eraser";
    default:
        return "Unknown";
    }
}

QString tabletEventDataToString(const TabletEventData& data)
{
    const QString s = "Tablet event type: " + tabletEventTypeToString(data.eventType) + ", " +
            "Pointer type: " + tabletEventPointerTypeToString(data.pointerType) + ", " +
            "View pos: " + QString::number(data.xViewPos, 'g', 8) + ", " + QString::number(data.yViewPos, 'g', 8) + ", " +
            "Scene pos: " + QString::number(data.xScenePos, 'g', 8) + ", " + QString::number(data.yScenePos, 'g', 8) + ", " +
            "Pressure: " + QString::number(data.pressure, 'g', 8) + ", " +
            "Tangential pressure: " + QString::number(data.tangentialPressure, 'g', 8) + ", " +
            "Rotation: " + QString::number(data.rotation, 'g', 8) + ", " +
            "Tilt: " + QString::number(data.xTilt, 'g', 8) + ", " + QString::number(data.yTilt, 'g', 8);
    return s;
}

const int CustomTabletEvent::customEventId = QEvent::registerEventType();

CustomTabletEvent::CustomTabletEvent(const TabletEventData& data, QWidget* sourceWidget) : QGraphicsSceneEvent{static_cast<QEvent::Type>(customEventId)}, m_data{data}
{
    setWidget(sourceWidget);
}

const TabletEventData& CustomTabletEvent::getData() const
{
    return m_data;
}

}

}
