#include "localization.h"

#include <cassert>
#include <vector>

#include <QApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
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
    const bool installedTranslator{application->installTranslator(translator)};
    assert(installedTranslator);
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
        if(translator->load(file, directory)) {
            installTranslator(application, translator);
        } else {
            assert(0 && "Failed to load translator"); // TODO this can happen if the translator is just empty, apparently, or it doesn't do lookup
        }
    };
    loadAndAddTranslator(qtTranslationFilePrefix + locale.toLower(), getQtTranslationResourceDirectory());
    loadAndAddTranslator(qtBaseTranslationFilePrefix + locale.toLower(), getQtTranslationResourceDirectory());
    loadAndAddTranslator(geometrizeTranslationFilePrefix + locale.toLower(), getAppTranslationResourceDirectory());
}

QIcon getFlagIconForIsoCode(const QString& isoCode)
{
    static const QIcon errorIcon{errorFlagResourcePath};

    QString code{isoCode};
    if(code.length() < 2) {
        return errorIcon;
    }

    if(code.length() > 2) {
        code.truncate(2); // For now only look at the language code (for now)
    }

    const QIcon languageIcon{flagIconResourceDirectory + code + flagIconFileExtension};
    if(!languageIcon.isNull()) {
        return languageIcon;
    }

    return errorIcon;
}

}
