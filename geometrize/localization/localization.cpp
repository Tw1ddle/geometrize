#include "localization.h"

#include <cassert>
#include <vector>

#include <QApplication>
#include <QDirIterator>
#include <QFileInfo>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QTranslator>

#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace
{

// Utility for keeping track of installed translators
std::vector<QTranslator*>& getTranslators()
{
    static std::vector<QTranslator*> translators;
    return translators;
}

void installTranslator(QCoreApplication* application, QTranslator* translator)
{
    std::vector<QTranslator*>& translators{getTranslators()};
    application->installTranslator(translator);
    translators.push_back(translator);
}

void purgeTranslators(QCoreApplication* application)
{
    std::vector<QTranslator*>& translators{getTranslators()};
    for(QTranslator* translator : translators) {
        const bool removed{application->removeTranslator(translator)};
        Q_UNUSED(removed);
        assert(removed);
        delete translator;
    }
    translators.clear();
}

}

const QString flagIconResourceDirectory{":/flags/"};
const QString flagIconFileExtension{".png"};
const QString errorFlagResourcePath{":/icons/error.png"};

const QString qtTranslationFilePrefix{"qt_"};
const QString qtBaseTranslationFilePrefix{"qtbase_"};
const QString geometrizeTranslationFilePrefix{"geometrize_"};

QString getBinaryTranslationFileExtension()
{
    return ".qm";
}

QString getAppTranslationResourceDirectory()
{
    return ":/translations/app/";
}

QString getQtTranslationResourceDirectory()
{
    return ":/translations/qt/";
}

void setTranslatorsForLocale(const QString& locale)
{
    QCoreApplication* application{QCoreApplication::instance()};
    if(application == nullptr) {
        assert(0 && "Attempting to set application translator before app is created");
        return;
    }

    // Remove and destroy old translators
    purgeTranslators(application);

    // Install the new translators
    const auto loadAndAddTranslator = [&application](const QString& file, const QString& directory) {
        QTranslator* translator{new QTranslator()};
        translator->load(file, directory);
        installTranslator(application, translator);
    };
    loadAndAddTranslator(qtTranslationFilePrefix + locale + geometrize::getBinaryTranslationFileExtension(), getQtTranslationResourceDirectory());
    loadAndAddTranslator(qtBaseTranslationFilePrefix + locale + geometrize::getBinaryTranslationFileExtension(), getQtTranslationResourceDirectory());
    loadAndAddTranslator(geometrizeTranslationFilePrefix + locale + geometrize::getBinaryTranslationFileExtension(), getAppTranslationResourceDirectory());
}

QIcon getFlagIconForLocaleCode(const QString& localeCode)
{
    static const QIcon errorIcon{errorFlagResourcePath};
    assert(!errorIcon.isNull());

    // Should start with a minimum-two digit language code
    if(localeCode.length() < 2) {
        return errorIcon;
    }

    const auto iconExists = [](const QString& file) {
        const QString path{flagIconResourceDirectory + file + flagIconFileExtension};
        const QFileInfo info(path);
        return info.exists() && info.isFile();
    };

    const auto makeIcon = [](const QString& file) {
        const QIcon icon(flagIconResourceDirectory + file + flagIconFileExtension);
        assert(!icon.isNull());
        return icon;
    };

    // Try to find flag for locale code, with fallback
    // If "language_country_locale" fails, then "language_country" and lastly "language" will be tried
    const auto makeIconPaths = [](const QString& localeCode) {
        const QString separator{"_"};

        #if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
        const QStringList parts{localeCode.split(separator, QString::SkipEmptyParts)};
        #else
        const QStringList parts{localeCode.split(separator, Qt::SkipEmptyParts)};
        #endif

        QStringList paths;
        if(parts.length() >= 3) {
            paths.push_back(parts[0] + "_" + parts[1] + "_" + parts[2]);
        }
        if(parts.length() >= 2) {
            paths.push_back(parts[0] + "_" + parts[1]);
        }
        if(parts.length() >= 1) {
            paths.push_back(parts[0]);
        }
        return paths;
    };

    const QStringList paths{makeIconPaths(localeCode)};
    for(const auto& path : paths) {
        if(iconExists(path)) {
            return makeIcon(path);
        }
    }

    return errorIcon;
}

QLocale getGlobalPreferencesLocale()
{
    const auto& prefs = geometrize::preferences::getGlobalPreferences();

    const QString language{QString::fromStdString(prefs.getLanguageIsoCode())};
    const QString country{QString::fromStdString(prefs.getCountryIsoCode())};
    const QString script{QString::fromStdString(prefs.getScriptIsoCode())};

    QString localeCode = "";
    if(!language.isEmpty()) {
        localeCode += language;
    }
    if(!script.isEmpty()) {
        localeCode += "_" + script;
    }
    if(!country.isEmpty()) {
        localeCode += "_" + country;
    }

    return QLocale(localeCode);
}

void setGlobalPreferencesForLocale(const QLocale& locale)
{
    auto& prefs = geometrize::preferences::getGlobalPreferences();

    const QStringList parts{locale.name().split("-")};

    if(parts.length() == 0 || parts.length() > 3) {
        assert(0 && "Failed to extract locale name, will fail to set preferences");
        return;
    }

    if(parts.length() == 1) {
        prefs.setLanguageIsoCode(parts[0].toStdString());
        return;
    }

    if(parts.length() == 2) {
        prefs.setLanguageIsoCode(parts[0].toStdString());
        prefs.setCountryIsoCode(parts[1].toStdString());
        return;
    }

    if(parts.length() == 3) {
        prefs.setLanguageIsoCode(parts[0].toStdString());
        prefs.setScriptIsoCode(parts[1].toStdString());

        const QStringList countryAndMaybeCodesetOrModifier{parts[2].split(".")};
        assert(countryAndMaybeCodesetOrModifier.length() > 0 && countryAndMaybeCodesetOrModifier.length() <= 3);

        if(countryAndMaybeCodesetOrModifier.length() > 0) {
            prefs.setCountryIsoCode(countryAndMaybeCodesetOrModifier[0].toStdString());
        }
        return;
    }
}

QStringList getSupportedLocaleCodes()
{
    QStringList supportedLocaleCodes;
    QDirIterator it(geometrize::getAppTranslationResourceDirectory());
    while (it.hasNext()) {
        it.next();
        QString fileName{it.fileName()};
        const QString localeCode{fileName.remove("geometrize_").remove(geometrize::getBinaryTranslationFileExtension())};
        supportedLocaleCodes.append(localeCode);
    }
    supportedLocaleCodes.sort();
    return supportedLocaleCodes;
}

void setLocaleAndUserInterfaceLanguage(const QString& isoCode)
{
    QString code = isoCode;
    if(code == "pt") {
        // NOTE hack - the default Portuguese translations are pt_PT not pt_BR, we make that explicit here
        // This is because Qt defaults to Brazilian version, whereas we preferred to default to Portugal
        code = "pt_PT";
    }

    geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
    prefs.setLanguageIsoCode(code.toStdString());
    QLocale::setDefault(QLocale(code));
    geometrize::setTranslatorsForLocale(code);
}

}
