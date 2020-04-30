#pragma once

#include <QGraphicsSceneEvent>
#include <QString>

class QWidget;

namespace geometrize
{

namespace scene
{

/**
 * @brief The TabletEventType enum represents the ids of tablet events the Wacom pen/tablet generates
 */
enum class TabletEventType
{
    Move,
    Press,
    Release,
    EnterProximity, // Wacom tablets/pens don't have to touch the screen to send motion, just be within proximity
    LeaveProximity,
    Unknown
};

/**
 * @brief tabletEventTypeToString Helper function to convert a tablet event type enum value to a string
 * @param type The tablet event type
 * @return String name of the tablet event type
 */
QString tabletEventTypeToString(const TabletEventType& type);

/**
 * @brief The TabletEventPointerType enum represents the type of pointer generating the tablet event
 */
enum class TabletEventPointerType
{
    UnknownPointer,
    Pen,
    Cursor,
    Eraser
};

/**
 * @brief tabletEventPointerTypeToString Helper function to convert a tablet event pointer type enum value to a string
 * @param type The tablet event pointer type
 * @return String name of the tablet pointer event type
 */
QString tabletEventPointerTypeToString(const TabletEventPointerType& type);

/**
 * @brief The TabletEventData struct represents data produced from a Wacom pen/tablet event (in the context of a graphics view)
 */
struct TabletEventData
{
    TabletEventData() = default;
    ~TabletEventData() = default;
    TabletEventData& operator=(const TabletEventData&) = default;
    TabletEventData(const TabletEventData&) = default;

    TabletEventType eventType{TabletEventType::Unknown};
    TabletEventPointerType pointerType{TabletEventPointerType::UnknownPointer};
    float xViewPos{0.0f};
    float yViewPos{0.0f};
    float xScenePos{0.0f};
    float yScenePos{0.0f};
    float pressure{0.0f};
    float tangentialPressure{0.0f};
    float rotation{0.0f};
    float xTilt{0.0f};
    float yTilt{0.0f};
};

/**
 * @brief tabletEventDataToString Helper function to convert a structure of tablet event data to a string
 * @param tabletEventData The tablet event data type
 * @return A human-readable string describing the tablet event data
 */
QString tabletEventDataToString(const TabletEventData& data);

/**
 * @brief The CustomTabletEvent class represents a tablet event (specifically from a Wacom Pro pen)
 * that can be passed down to items in a graphics scene
 */
class CustomTabletEvent : public QGraphicsSceneEvent
{
public:
    /**
     * @brief CustomTabletEventTypeId is the id for the corresponding custom tablet event type
     */
    static const int customEventId;

    /**
     * @brief CustomTabletEvent Creates a custom tablet event
     * @param data The data that the tablet event holds
     * @param sourceWidget The widget that the event came from
     */
    CustomTabletEvent(const TabletEventData& data, QWidget* sourceWidget);
    ~CustomTabletEvent() = default;

    const TabletEventData& getData() const;

private:
    const TabletEventData m_data;
};

}

}
