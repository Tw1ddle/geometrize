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

void AnalyticsWrapper::onLogoImageTaskFinished(const int shapeCount, const float durationMs)
{
    QVariantMap map;
    map["shape_count"] = shapeCount;
    m_analytics.sendEvent("logo_image_task_finished", "task_timed", "task_timed", durationMs, map);
}

}

}
