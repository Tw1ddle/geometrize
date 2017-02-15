#include "analyticswrapper.h"

namespace geometrize
{

namespace analytics
{

AnalyticsWrapper::AnalyticsWrapper()
{

}

void AnalyticsWrapper::startSession()
{
    m_analytics.startSession();
}

void AnalyticsWrapper::endSession()
{
    m_analytics.endSession();
}

void AnalyticsWrapper::onLaunch()
{
    m_analytics.sendEvent("launch", "app_launch");
}

void AnalyticsWrapper::onLogoImageJobFinished(const int shapeCount, const float durationMs)
{
    QVariantMap map;
    map["shape_count"] = shapeCount;
    m_analytics.sendEvent("logo_image_job_finished", "job_timed", "job_timed", durationMs, map);
}

/**
 * @brief sendEvent Send an analytics event.
 * @param category A name you supply to group the object you want to track e.g. "Videos".
 * @param action The name of the type of event or interaction you want to track e.g. "Play" button clicks. Within a category.
 * @param label Additional information for events you want to track, such as "movie name" for a "Videos" category "Play" action.
 * @param value A numerical value, such as a video load time.
 * @param customValues A map of custom values.
 */

}

}
