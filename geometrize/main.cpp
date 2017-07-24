#include "dialog/launchwindow.h"

#include <functional>
#include <string>

#include <QApplication>
#include <QLocale>
#include <QString>
#include <QStringList>

#include "analytics/analyticswrapper.h"
#include "cli/commandlineparser.h"
#include "common/sharedapp.h"
#include "localization/localization.h"
#include "logger/logmessagehandlers.h"
#include "preferences/globalpreferences.h"
#include "version/versioninfo.h"

namespace {

void setupSettingsFields()
{
    // Do not modify these
    const QString ORGANIZATION_NAME{"Sam Twidale"}; // The development organization's name.
    const QString ORGANIZATION_DOMAIN{"samcodes.co.uk"}; // The development organization's website domain.
    const QString APPLICATION_NAME{"Geometrize"}; // The application name.
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QCoreApplication::setApplicationVersion(geometrize::version::getApplicationVersionString()); // This can change
}

void installMessageHandlers()
{
    qInstallMessageHandler(geometrize::log::handleLogMessages);
}

void setLocale(const std::string& languageCode)
{
    // TODO sets the locale based on the global preference, should only use it if one is set (by default there should be no preference, so it should use the system?)
    QLocale::setDefault(QLocale(QString::fromStdString(languageCode)));
    geometrize::setTranslatorsForLocale(QLocale::system().name());
}

void setupAnalytics()
{
    geometrize::analytics::AnalyticsWrapper analytics;
    analytics.startSession();
    analytics.onLaunch();
}

int runAppConsoleMode(QApplication& app)
{
    return geometrize::cli::runApp(app);
}

int runAppGuiMode(QApplication& app)
{
    geometrize::dialog::LaunchWindow w;
    w.show();
    return app.exec();
}

std::function<int(QApplication&)> resolveLaunchFunction(const QStringList& arguments)
{
    if(geometrize::cli::shouldRunInConsoleMode(arguments)) {
        return ::runAppConsoleMode;
    }
    return ::runAppGuiMode;
}

}

int main(int argc, char* argv[])
{
    setupSettingsFields();
    installMessageHandlers();

    QApplication app(argc, argv);

    geometrize::preferences::GlobalPreferences& prefs{geometrize::common::app::SharedApp::get().getGlobalPreferences()};
    setLocale(prefs.getLanguageIsoCode());

    setupAnalytics();

    const auto run{resolveLaunchFunction(app.arguments())};
    return run(app);
}
