#pragma once

#include "analytics.h"

namespace geometrize
{

namespace analytics
{

/**
 * @brief The AnalyticsWrapper class implements specific analytics events for use throughout an application.
 */
class AnalyticsWrapper
{
public:
    AnalyticsWrapper();
    AnalyticsWrapper& operator=(const AnalyticsWrapper&) = delete;
    AnalyticsWrapper(const AnalyticsWrapper&) = delete;
    ~AnalyticsWrapper() = default;

    void startSession();
    void endSession();

    void onLaunch(); // TODO log mode it was launched in e.g. command line, GUI, executing script etc?

private:
    Analytics m_analytics;
};

}

}
