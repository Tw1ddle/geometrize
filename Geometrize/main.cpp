#include "dialog/launchwindow.h"

#include <QApplication>
#include <QDebug>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

#include "constants.h"
#include "sharedapp.h"
#include "recentitems.h"

/// ChaiScript function for printing to console
void printOutput(const std::string &t_str)
{
    qDebug(t_str.c_str());
}

/// ChaiScript test function
std::string helloWorld(const std::string& t_name)
{
    return "Hello " + t_name + "!";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
    chai.add(chaiscript::fun(&printOutput), "printOutput");
    chai.add(chaiscript::fun(&helloWorld), "helloWorld");
    chai.eval("use(\"../Geometrize/scripts/hello_world.chai\") \n printOutput(\"Hello World\")");

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
