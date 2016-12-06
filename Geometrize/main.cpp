#include "dialog/launchwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.processEvents();

    geometrize::dialog::LaunchWindow w;
    w.show();

    return a.exec();
}
