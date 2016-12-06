#include "dialog/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.processEvents();

    geometrize::dialog::MainWindow w;
    w.show();

    return a.exec();
}
