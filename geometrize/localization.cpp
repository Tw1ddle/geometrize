#include "localization.h"

#include <cassert>

#include <QApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QTranslator>

namespace geometrize
{

const QString flagIconResourceDirectory{":/flags/"};
const QString flagIconFileExtension{".png"};
const QString errorFlagResourcePath{":/icons/error.png"};

const QString appTranslationResourceDirectory{":/translations/app/"};
const QString qtTranslationResourceDirectory{":/translations/qt/"};

const QString qtTranslationPrefix{"qt_"};
const QString qtBaseTranslationPrefix{"qt_base"};

const QString binaryTranslationFileExtension{".qm"};

void installTranslatorsForLocale(QCoreApplication& application, const QString& locale)
{
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

QIcon getFlagIconForIsoCode(const QString& isoCode)
{
    static QIcon errorIcon{errorFlagResourcePath};

    QString code{isoCode};
    if(code.length() < 2) {
        return errorIcon;
    }

    if(code.length() > 2) {
        code.truncate(2);
    }

    const QIcon icon{flagIconResourceDirectory + code + flagIconFileExtension};
    if(icon.isNull()) {
        return errorIcon;
    }
    return icon;
}

}
