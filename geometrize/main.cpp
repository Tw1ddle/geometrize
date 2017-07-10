#include "dialog/launchwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLocale>
#include <QString>

#include "analytics/analyticswrapper.h"
#include "common/sharedapp.h"
#include "commandlineparser.h"
#include "localization.h"
#include "logmessagehandlers.h"
#include "preferences/globalpreferences.h"
#include "runguard.h"
#include "versioninfo.h"

void setupSettingsFields()
{
    // Do not modify these
    const QString ORGANIZATION_NAME{"Sam Twidale"}; // The development organization's name.
    const QString ORGANIZATION_DOMAIN{"samcodes.co.uk"}; // The development organization's website domain.
    const QString APPLICATION_NAME{"Geometrize"}; // The application name.
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    // These can change
    QCoreApplication::setApplicationVersion(geometrize::version::getApplicationVersionString());
}

int main(int argc, char* argv[])
{
    setupSettingsFields();

    qInstallMessageHandler(geometrize::log::handleLogMessages);
    QApplication app(argc, argv);

    // TODO sets the locale based on the global preference, should only use it if one is set (by default there should be no preference, so it should use the system?)
    geometrize::preferences::GlobalPreferences& prefs{geometrize::common::app::SharedApp::get().getGlobalPreferences()};
    const std::string languageCode{prefs.getLanguageIsoCode()};
    QLocale::setDefault(QLocale(QString::fromStdString(languageCode)));
    geometrize::setTranslatorsForLocale(QLocale::system().name());

    QCommandLineParser parser;
    const geometrize::cli::CommandLineResult cliSetup{geometrize::cli::setupCommandLineParser(parser, app.arguments())};
    const geometrize::cli::CommandLineResult options{geometrize::cli::handleArgumentPairs(parser)};
    const geometrize::cli::CommandLineResult positionals{geometrize::cli::handlePositionalArguments(parser.positionalArguments())};

    geometrize::analytics::AnalyticsWrapper analytics;
    analytics.startSession();
    analytics.onLaunch();

    // Open launcher window if there isn't an instance of Geometrize already running
    geometrize::RunGuard runGuard("geometrize_run_guard_key");
    geometrize::dialog::LaunchWindow w;
    if(runGuard.tryToRun()) {
        w.show();
    }

    return app.exec();
}
