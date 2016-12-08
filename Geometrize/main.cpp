#include "dialog/launchwindow.h"

#include <QApplication>

#include "constants.h"
#include "sharedapp.h"
#include "recentitems.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Setup QSettings default fields (do not modify these)
    const QString ORGANIZATION_NAME{"Sam Twidale"}; // The development organization's name.
    const QString ORGANIZATION_DOMAIN{"samcodes.co.uk"}; // The development organization's website domain.
    const QString APPLICATION_NAME{"Geometrize"}; // The application name.
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    a.processEvents();

    // Note first-time initialization of shared app singleton
    geometrize::SharedApp& app{geometrize::SharedApp::get()};

    geometrize::dialog::LaunchWindow w;

    // TODO connect recent files to list widget signals

    w.show();

    return a.exec();
}
