#include "dialog/launchwindow.h"

#include <QApplication>
#include <QCommandLineParser>

#include "chaiscript/chaiscript.hpp"

#include "script/chaiscriptcreator.h"
#include "constants.h"
#include "util.h"
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

void setupCommandLineParser(QCommandLineParser& parser, const QStringList& arguments)
{
    parser.setApplicationDescription(geometrize::constants::Strings::getApplicationDescription());
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"s", "script"}, QCoreApplication::translate("main", "Path to ChaiScript script file")}
    });

    parser.process(arguments);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setupSettingsFields();

    QCommandLineParser parser;
    setupCommandLineParser(parser, app.arguments());

    // TODO add open-with support for files, scripts, urls etc

    geometrize::dialog::LaunchWindow w;

    // If widget is larger than the main display, resize so it fits
    //const QRect mainScreenSize{QApplication::desktop()->availableGeometry()};
    //if(w.width() > mainScreenSize.width() || w.height() > mainScreenSize.height()) {
    //    w.resize(mainScreenSize.width(), mainScreenSize.height());
    //}

    w.show();

    return app.exec();
}
