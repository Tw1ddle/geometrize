#include "dialog/launchwindow.h"

#include <QApplication>
#include <QDebug>

#include "chaiscript/chaiscript.hpp"

#include "script/chaiscriptcreator.h"
#include "constants.h"
#include "searchpaths.h"
#include "sharedapp.h"
#include "recentitems.h"
#include "util.h"

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

    geometrize::script::createChaiScript();

    qDebug() << QString::fromStdString(geometrize::searchpaths::getApplicationDirectoryPath());

    a.processEvents();

    // TODO remove? split into recent files etc...
    // Note first-time initialization of shared app singleton
    geometrize::app::SharedApp& app{geometrize::app::SharedApp::get()};

    geometrize::dialog::LaunchWindow w;

    // TODO connect recent files to list widget signals

    w.show();

    return a.exec();
}
