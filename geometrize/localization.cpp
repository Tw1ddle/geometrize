#include "localization.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

namespace geometrize
{

void setupLocalization(QApplication& application)
{
    const QString locale{QLocale::system().name()};

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtTranslator);

    QTranslator geometrizeTranslator;
    geometrizeTranslator.load("geometrize_" + locale); // Should fall back to filename without country suffix if necessary etc
    application.installTranslator(&geometrizeTranslator);
}

}
