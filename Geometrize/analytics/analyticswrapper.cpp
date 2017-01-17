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

}

}
