#include "analytics.h"

#include "ganalytics.h"

#include <QLocale>

namespace geometrize
{

namespace analytics
{

class Analytics::AnalyticsImpl
{
public:
    AnalyticsImpl(Analytics* pQ) : q{pQ}, m_analytics{"UA-58489066-5"}
    {
        m_analytics.setLanguage(QLocale::system().name());
        m_analytics.setSendInterval(30000);
    }
    ~AnalyticsImpl()
    {
        endSession();
    }
    AnalyticsImpl operator=(const AnalyticsImpl&) = delete;
    AnalyticsImpl(const AnalyticsImpl&) = delete;

    void startSession()
    {
        m_analytics.startSession();
    }

    void endSession()
    {
        m_analytics.endSession();
    }

    void sendEvent(const QString& category,
                   const QString& action,
                   const QString& label,
                   const QVariant& value,
                   const QVariantMap& customValues)
    {
        m_analytics.sendEvent(category, action, label, value, customValues);
    }

private:
    Analytics* q;
    GAnalytics m_analytics;
};

Analytics::Analytics() : d{std::make_unique<Analytics::AnalyticsImpl>(this)}
{
}

Analytics::~Analytics()
{
}

void Analytics::startSession()
{
    d->startSession();
}

void Analytics::endSession()
{
    d->endSession();
}

void Analytics::sendEvent(const QString& category,
               const QString& action,
               const QString& label,
               const QVariant& value,
               const QVariantMap& customValues)
{
    d->sendEvent(category, action, label, value, customValues);
}

}

}
