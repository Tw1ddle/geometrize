#include "localization.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QTranslator>

namespace geometrize
{

void setupLocalization(QApplication& application, const QString& locale)
{
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtTranslator);

    QTranslator geometrizeTranslator;
    geometrizeTranslator.load("geometrize_" + locale); // Should fall back to filename without country suffix if necessary etc
    application.installTranslator(&geometrizeTranslator);
}

}
