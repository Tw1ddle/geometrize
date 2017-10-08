#include "localization.h"

#include <cassert>
#include <vector>

#include <QApplication>
#include <QFileInfo>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTranslator>

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
    loadAndAddTranslator(qtTranslationFilePrefix + locale.toLower() + geometrize::getBinaryTranslationFileExtension(), getQtTranslationResourceDirectory());
    loadAndAddTranslator(qtBaseTranslationFilePrefix + locale.toLower() + geometrize::getBinaryTranslationFileExtension(), getQtTranslationResourceDirectory());
    loadAndAddTranslator(geometrizeTranslationFilePrefix + locale.toLower() + geometrize::getBinaryTranslationFileExtension(), getAppTranslationResourceDirectory());
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
        const QStringList parts{localeCode.split(separator, QString::SkipEmptyParts)};

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

}
