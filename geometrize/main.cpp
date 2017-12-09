#include "dialog/launchwindow.h"

#include <functional>
#include <string>

#include <QApplication>
#include <QIcon>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QtGlobal>

#include "cli/commandlineparser.h"
#include "common/uiactions.h"
#include "dialog/launchwindow.h"
#include "dialog/welcomewindow.h"
#include "localization/localization.h"
#include "logger/logmessagehandlers.h"
#include "preferences/globalpreferences.h"
#include "version/versioninfo.h"

namespace {

void setApplicationSettingsFields()
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

void setLocale(const QStringList& arguments)
{
    const std::string languageCode = [&arguments]() {
        const std::string overrideCode{geometrize::cli::getOverrideLocaleCode(arguments)};
        if(!overrideCode.empty()) {
            return overrideCode;
        }

        const auto& prefs = geometrize::preferences::getGlobalPreferences();
        return prefs.getLanguageIsoCode();
    }();

    // TODO should only use this if a locale is set (by default there should be no preference, so it should use the system?)
    QLocale::setDefault(QLocale(QString::fromStdString(languageCode)));
    geometrize::setTranslatorsForLocale(QString::fromStdString(languageCode));
}

int runAppConsoleMode(QApplication& app)
{
    return geometrize::cli::runApp(app);
}

#ifdef Q_OS_WINRT

int runAppGuiModeUwp(QApplication& app)
{
    geometrize::common::ui::openLaunchWindow(); // No welcome screen in the UWP build
    return app.exec();
}

#endif

int runAppGuiModeDesktop(QApplication& app)
{
    const auto& prefs = geometrize::preferences::getGlobalPreferences();
	if (prefs.shouldShowWelcomeScreenOnLaunch()) {
        geometrize::common::ui::openWelcomePage(); // Opens launch window on close
    } else {
        geometrize::common::ui::openLaunchWindow();
    }

    return app.exec();
}

std::function<int(QApplication&)> resolveLaunchFunction(const QStringList& arguments)
{
    if(geometrize::cli::shouldRunInConsoleMode(arguments)) {
        return ::runAppConsoleMode;
    }

#ifdef Q_OS_WINRT
    return ::runAppGuiModeUwp;
#else
    return ::runAppGuiModeDesktop;
#endif
}

}

int main(int argc, char* argv[])
{
    setApplicationSettingsFields();
    installMessageHandlers();

    QApplication app(argc, argv);

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    // Some Linux/Mac taskbars/DEs use the application window icon to set the taskbar icon
    // So we explicitly set the window icon here
    app.setWindowIcon(QIcon(":/logos/logo_small.png"));
#endif

    const QStringList arguments{app.arguments()};
    setLocale(arguments);

    const auto run = resolveLaunchFunction(arguments);
    return run(app);
}
