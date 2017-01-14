#include "dialog/launchwindow.h"

#include <assert.h>

#include <QDebug>

#include <QApplication>
#include <QCommandLineParser>

#include "commandlineparser.h"
#include "constants.h"
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
    QApplication app(argc, argv);

    setupSettingsFields();

    QCommandLineParser parser;
    geometrize::cli::setupCommandLineParser(parser, app.arguments());
    geometrize::cli::handlePositionalArguments(parser.positionalArguments());

    geometrize::dialog::LaunchWindow w;
    w.show();

    return app.exec();
}
