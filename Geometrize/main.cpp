#include "forms/mainwindow.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // TODO
    QPixmap splashPixmap{":/splash.png"};
    QSplashScreen splash(splashPixmap);
    splash.show();
    a.processEvents();

    MainWindow w;
    w.show();

    return a.exec();
}
