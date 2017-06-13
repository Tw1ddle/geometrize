#include "localization.h"

#include <cassert>

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QTranslator>

namespace geometrize
{

const QString appTranslationResourceDirectory{":/translations/app/"};
const QString qtTranslationResourceDirectory{":/translations/qt/"};

const QString qtTranslationPrefix{"qt_"};
const QString qtBaseTranslationPrefix{"qt_base"};

void setupLocalization(QApplication& application, const QString& locale)
{
    // TODO refactor into member variables on an app class or localizer object

    QTranslator* qtTranslator = new QTranslator();
    if(qtTranslator->load(qtTranslationPrefix + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        application.installTranslator(qtTranslator);
    }

    QTranslator* qtBaseTranslator = new QTranslator();
    if(qtBaseTranslator->load(qtBaseTranslationPrefix+ locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        application.installTranslator(qtBaseTranslator);
    }

    QTranslator* geometrizeTranslator = new QTranslator();
    if(geometrizeTranslator->load(locale, appTranslationResourceDirectory)) {
        application.installTranslator(geometrizeTranslator);
    }
}

}
