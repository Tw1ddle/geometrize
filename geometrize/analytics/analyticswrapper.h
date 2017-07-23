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

    /**
     * @brief startSession Event recorded when the app is launched.
     */
    void startSession();

    /**
     * @brief endSession Event recorded when the app quits normally.
     */
    void endSession();

    /**
     * @brief onLaunch Event recorded when the app is launched.
     */
    void onLaunch();

    /**
     * @brief onLogoImageJobFinished Event recorded when the image job embedded in the lauch image completes.
     */
    void onLogoImageJobFinished(int shapeCount, float durationMs);

private:
    Analytics m_analytics;
};

}

}
