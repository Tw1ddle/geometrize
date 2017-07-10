#pragma once

#include <memory>

#include <QString>
#include <QVariant>
#include <QVariantMap>

namespace geometrize
{

namespace analytics
{

/**
 * @brief The Analytics class provides a simple frontend for a user analytics service (like Google Analytics).
 * Should be used sparingly (and provide a user option to disable it) to get some insight on how the app is used.
 */
class Analytics
{
public:
    Analytics();
    Analytics& operator=(const Analytics&) = delete;
    Analytics(const Analytics&) = delete;
    ~Analytics();

    /**
     * @brief startSession Start the analytics session.
     */
    void startSession();

    /**
     * @brief endSession Stop the analytics session.
     */
    void endSession();

    /**
     * @brief sendEvent Send an analytics event.
     * @param category A name you supply to group the object you want to track e.g. "Videos".
     * @param action The name of the type of event or interaction you want to track e.g. "Play" button clicks. Within a category.
     * @param label Additional information for events you want to track, such as "movie name" for a "Videos" category "Play" action.
     * @param value A numerical value, such as a video load time.
     * @param customValues A map of custom values.
     */
    void sendEvent(const QString& category,
                   const QString& action,
                   const QString& label = QString(),
                   const QVariant& value = QVariant(),
                   const QVariantMap& customValues = QVariantMap());

private:
    class AnalyticsImpl;
    std::unique_ptr<AnalyticsImpl> d;
};

}

}
