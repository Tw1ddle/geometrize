#include "forms/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.processEvents();

    MainWindow w;
    w.show();

    return a.exec();
}
