#include "localization.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

namespace geometrize
{

void setupLocalization(QApplication& application)
{
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtTranslator);

    QTranslator geometrizeTranslator;
    geometrizeTranslator.load("geometrize_" + QLocale::system().name());
    application.installTranslator(&geometrizeTranslator);
}

}
