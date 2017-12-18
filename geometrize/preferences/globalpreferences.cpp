#include "globalpreferences.h"

#include <cassert>
#include <fstream>
#include <ostream>

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

#include "cereal/archives/json.hpp"

#include "common/util.h"
#include "serialization/globalpreferencesdata.h"

namespace geometrize
{

namespace preferences
{

std::string getGlobalPreferencesConfigPath()
{
    const QString configFilename{"global_preferences.json"};
    const QString globalPreferencesFolder{QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation)};
    const QString globalPreferencesPath{QDir(globalPreferencesFolder).filePath(configFilename)};
    const std::string globalPreferencesPathStr{globalPreferencesPath.toStdString()};
    return globalPreferencesPathStr;
}

geometrize::preferences::GlobalPreferences& getGlobalPreferences()
{
    static geometrize::preferences::GlobalPreferences prefs{};
    return prefs;
}

class GlobalPreferences::GlobalPreferencesImpl
{
public:
    GlobalPreferencesImpl()
    {
        load(getGlobalPreferencesConfigPath());
    }

    ~GlobalPreferencesImpl()
    {
        // NOTE this should really be done after everything else is done writing to preferences
        save(getGlobalPreferencesConfigPath());
    }

    GlobalPreferencesImpl& operator=(const GlobalPreferencesImpl&) = default;
    GlobalPreferencesImpl(const GlobalPreferencesImpl&) = default;

    bool load(const std::string& filePath)
    {
        std::ifstream input(filePath);
        try {
            cereal::JSONInputArchive archive{input};
            performArchival(archive);
        } catch(...) {
            assert(0 && "Failed to read global preferences");
            return false;
        }
        return true;
    }

    bool save(const std::string& filePath)
    {
        // Create the directory at the filepath if it does not already exist
        // This is necessary because the output archive will not create any missing directories
        const QFileInfo info(QString::fromStdString(filePath));
        const QDir dir(info.absoluteDir());
        if(!dir.exists() && !dir.mkpath(dir.absolutePath())) {
            assert(0 && "Failed to create directory in which to save global preferences");
            return false;
        }

        std::ofstream output(filePath);
        try {
            cereal::JSONOutputArchive archive{output};
            performArchival(archive);
        } catch(...) {
            assert(0 && "Failed to write global preferences");
            return false;
        }
        return true;
    }

    unsigned int getApplicationLaunchCount() const
    {
        return m_applicationLaunchCount;
    }

    void incrementApplicationLaunchCount()
    {
        m_applicationLaunchCount++;
    }

    bool shouldShowWelcomeScreenOnLaunch() const
    {
        return m_shouldShowWelcomeScreenOnLaunch;
    }

    void setShouldShowWelcomeScreenOnLaunch(const bool show)
    {
        m_shouldShowWelcomeScreenOnLaunch = show;
    }

    bool shouldGeometrizeAppLogoOnLaunch() const
    {
        return m_shouldGeometrizeAppLogoOnLaunch;
    }

    void setShouldGeometrizeAppLogoOnLaunch(const bool geometrizeLogo)
    {
        m_shouldGeometrizeAppLogoOnLaunch = geometrizeLogo;
    }

    bool shouldPopulateRecentItemsOnLaunch() const
    {
        return m_shouldPopulateRecentItemsOnLaunch;
    }

    void setShouldPopulateRecentItemsOnLaunch(const bool populateRecents)
    {
        m_shouldPopulateRecentItemsOnLaunch = populateRecents;
    }

    bool shouldPopulateTemplatesOnLaunch() const
    {
        return m_shouldPopulateTemplatesOnLaunch;
    }

    void setShouldPopulateTemplatesOnLaunch(const bool populateTemplates)
    {
        m_shouldPopulateTemplatesOnLaunch = populateTemplates;
    }

    bool shouldShowLaunchConsoleByDefault() const
    {
        return m_shouldShowLaunchConsoleByDefault;
    }

    void setShouldShowLaunchConsoleByDefault(const bool showConsole)
    {
        m_shouldShowLaunchConsoleByDefault = showConsole;
    }

    bool shouldShowImageTaskVectorViewByDefault() const
    {
        return m_shouldShowImageTaskVectorViewByDefault;
    }

    void setShouldShowImageTaskVectorViewByDefault(const bool showVectorView)
    {
        m_shouldShowImageTaskVectorViewByDefault = showVectorView;
    }

    bool shouldShowImageTaskConsoleByDefault() const
    {
        return m_shouldShowImageTaskConsoleByDefault;
    }

    void setShouldShowImageTaskConsoleByDefault(const bool showConsole)
    {
        m_shouldShowImageTaskConsoleByDefault = showConsole;
    }

    bool shouldShowImageTaskScriptEditorByDefault() const
    {
        return m_shouldShowImageTaskScriptEditorByDefault;
    }

    void setShouldShowImageTaskScriptEditorByDefault(const bool showScriptEditor)
    {
        m_shouldShowImageTaskScriptEditorByDefault = showScriptEditor;
    }

    bool shouldShowImageTaskPixmapViewByDefault() const
    {
        return m_shouldShowImageTaskPixmapViewByDefault;
    }

    void setShouldShowImageTaskPixmapViewByDefault(const bool showPixmapView)
    {
        m_shouldShowImageTaskPixmapViewByDefault = showPixmapView;
    }

    void setImageTaskResizeThreshold(const std::uint32_t width, const std::uint32_t height)
    {
        m_imageTaskResizeThreshold.first = width;
        m_imageTaskResizeThreshold.second = height;
    }

    std::pair<std::uint32_t, std::uint32_t> getImageTaskResizeThreshold() const
    {
        return m_imageTaskResizeThreshold;
    }

    bool isImageTaskImageResizeEnabled() const
    {
        return m_imageTaskResizeEnabled;
    }

    void setImageTaskImageResizeEnabled(const bool enabled)
    {
        m_imageTaskResizeEnabled = enabled;
    }

    std::uint32_t getImageTaskMaxThreads() const
    {
        return m_imageTaskMaxThreads;
    }

    void setImageTaskMaxThreads(const std::uint32_t maxThreads)
    {
        m_imageTaskMaxThreads = maxThreads;
    }

    std::string getLanguageIsoCode() const
    {
        return m_languageIsoCode;
    }

    void setLanguageIsoCode(const std::string& languageIsoCode)
    {
        m_languageIsoCode = languageIsoCode;
    }

    std::string getScriptIsoCode() const
    {
        return m_scriptIsoCode;
    }

    void setScriptIsoCode(const std::string& scriptIsoCode)
    {
        m_scriptIsoCode = scriptIsoCode;
    }

    std::string getCountryIsoCode() const
    {
        return m_countryIsoCode;
    }

    void setCountryIsoCode(const std::string& countryIsoCode)
    {
        m_countryIsoCode = countryIsoCode;
    }

    std::string getLocaleName() const
    {
        std::string localeName = m_languageIsoCode;
        if(!m_scriptIsoCode.empty()) {
            localeName += "_" + m_scriptIsoCode;
        }
        if(!m_countryIsoCode.empty()) {
            localeName += "_" + m_countryIsoCode;
        }
        return localeName;
    }

    void setLocaleName(const std::string& localeName)
    {
        const std::vector<std::string> components{geometrize::util::split(localeName, '_')};
        const std::size_t size{components.size()};

        std::string languageCode{""};
        std::string scriptCode{""};
        std::string countryCode{""};

        switch(size) {
        case 1:
            languageCode = components[0];
            break;
        case 2:
            languageCode = components[0];
            if(components[1].size() == 2) {
                countryCode = components[1];
            } else if(components[1].size() == 4) {
                scriptCode = components[1];
            } else {
                assert(0 && "First locale component was invalid");
            }
            break;
        case 3:
            languageCode = components[0];
            scriptCode = components[1];
            countryCode = components[2];
            break;
        default:
            assert(0 && "Failed to set invalid locale name");
            return;
        }

        setLanguageIsoCode(languageCode);
        setScriptIsoCode(scriptCode);
        setCountryIsoCode(countryCode);
    }

private:
    template <typename T>
    void performArchival(T& archive)
    {
        m_data.archive(archive,
           m_shouldShowWelcomeScreenOnLaunch,

           m_shouldGeometrizeAppLogoOnLaunch,
           m_shouldPopulateRecentItemsOnLaunch,
           m_shouldPopulateTemplatesOnLaunch,

           m_shouldShowLaunchConsoleByDefault,
           m_shouldShowImageTaskVectorViewByDefault,
           m_shouldShowImageTaskConsoleByDefault,
           m_shouldShowImageTaskScriptEditorByDefault,
           m_shouldShowImageTaskPixmapViewByDefault,

           m_imageTaskResizeEnabled,
           m_imageTaskResizeThreshold,
           m_imageTaskMaxThreads,

           m_languageIsoCode,
           m_scriptIsoCode,
           m_countryIsoCode);
    }

    serialization::GlobalPreferencesData m_data;

    unsigned int m_applicationLaunchCount{0};

    bool m_shouldShowWelcomeScreenOnLaunch{true};

    bool m_shouldGeometrizeAppLogoOnLaunch{true};
    bool m_shouldPopulateRecentItemsOnLaunch{true};
    bool m_shouldPopulateTemplatesOnLaunch{true};

    bool m_shouldShowLaunchConsoleByDefault{false};
    bool m_shouldShowImageTaskVectorViewByDefault{true};
    bool m_shouldShowImageTaskConsoleByDefault{false};
    bool m_shouldShowImageTaskScriptEditorByDefault{false};
    bool m_shouldShowImageTaskPixmapViewByDefault{false};

    bool m_imageTaskResizeEnabled{true};
    std::pair<std::uint32_t, std::uint32_t> m_imageTaskResizeThreshold{256, 256};
    std::uint32_t m_imageTaskMaxThreads{4};

    std::string m_languageIsoCode{"en"};
    std::string m_scriptIsoCode{"Latn"};
    std::string m_countryIsoCode{"US"};

};

GlobalPreferences::GlobalPreferences() : d{std::make_unique<GlobalPreferences::GlobalPreferencesImpl>()}
{
}

GlobalPreferences::~GlobalPreferences()
{
}

unsigned int GlobalPreferences::getApplicationLaunchCount() const
{
    return d->getApplicationLaunchCount();
}

void GlobalPreferences::incrementApplicationLaunchCount()
{
    d->incrementApplicationLaunchCount();
}

bool GlobalPreferences::load(const std::string& filePath)
{
    return d->load(filePath);
}

bool GlobalPreferences::save(const std::string& filePath)
{
    return d->save(filePath);
}

bool GlobalPreferences::shouldShowWelcomeScreenOnLaunch() const
{
    return d->shouldShowWelcomeScreenOnLaunch();
}

void GlobalPreferences::setShouldShowWelcomeScreenOnLaunch(const bool show)
{
    d->setShouldShowWelcomeScreenOnLaunch(show);
}

bool GlobalPreferences::shouldGeometrizeAppLogoOnLaunch() const
{
    return d->shouldGeometrizeAppLogoOnLaunch();
}

void GlobalPreferences::setShouldGeometrizeAppLogoOnLaunch(const bool geometrizeLogo)
{
    d->setShouldGeometrizeAppLogoOnLaunch(geometrizeLogo);
}

bool GlobalPreferences::shouldPopulateRecentItemsOnLaunch() const
{
    return d->shouldPopulateRecentItemsOnLaunch();
}

void GlobalPreferences::setShouldPopulateRecentItemsOnLaunch(const bool populateRecents)
{
    d->setShouldPopulateRecentItemsOnLaunch(populateRecents);
}

bool GlobalPreferences::shouldPopulateTemplatesOnLaunch() const
{
    return d->shouldPopulateTemplatesOnLaunch();
}

void GlobalPreferences::setShouldPopulateTemplatesOnLaunch(const bool populateTemplates)
{
    d->setShouldPopulateTemplatesOnLaunch(populateTemplates);
}

bool GlobalPreferences::shouldShowLaunchConsoleByDefault() const
{
    return d->shouldShowLaunchConsoleByDefault();
}

void GlobalPreferences::setShouldShowLaunchConsoleByDefault(const bool showConsole)
{
    d->setShouldShowLaunchConsoleByDefault(showConsole);
}

bool GlobalPreferences::shouldShowImageTaskVectorViewByDefault() const
{
    return d->shouldShowImageTaskVectorViewByDefault();
}

void GlobalPreferences::setShouldShowImageTaskVectorViewByDefault(const bool showVectorView)
{
    d->setShouldShowImageTaskVectorViewByDefault(showVectorView);
}

bool GlobalPreferences::shouldShowImageTaskConsoleByDefault() const
{
    return d->shouldShowImageTaskConsoleByDefault();
}

void GlobalPreferences::setShouldShowImageTaskConsoleByDefault(const bool showConsole)
{
    d->setShouldShowImageTaskConsoleByDefault(showConsole);
}

bool GlobalPreferences::shouldShowImageTaskScriptEditorByDefault() const
{
    return d->shouldShowImageTaskScriptEditorByDefault();
}

void GlobalPreferences::setShouldShowImageTaskScriptEditorByDefault(const bool showScriptEditor)
{
    d->setShouldShowImageTaskScriptEditorByDefault(showScriptEditor);
}

bool GlobalPreferences::shouldShowImageTaskPixmapViewByDefault() const
{
    return d->shouldShowImageTaskPixmapViewByDefault();
}

void GlobalPreferences::setShouldShowImageTaskPixmapViewByDefault(const bool showPixmapView)
{
    d->setShouldShowImageTaskPixmapViewByDefault(showPixmapView);
}

bool GlobalPreferences::isImageTaskImageResizeEnabled() const
{
    return d->isImageTaskImageResizeEnabled();
}

void GlobalPreferences::setImageTaskImageResizeEnabled(const bool enabled)
{
    d->setImageTaskImageResizeEnabled(enabled);
}

std::pair<std::uint32_t, std::uint32_t> GlobalPreferences::getImageTaskResizeThreshold() const
{
    return d->getImageTaskResizeThreshold();
}

void GlobalPreferences::setImageTaskResizeThreshold(const std::uint32_t width, const std::uint32_t height)
{
    d->setImageTaskResizeThreshold(width, height);
}

std::uint32_t GlobalPreferences::getImageTaskMaxThreads() const
{
    return d->getImageTaskMaxThreads();
}

void GlobalPreferences::setImageTaskMaxThreads(const std::uint32_t maxThreads)
{
    d->setImageTaskMaxThreads(maxThreads);
}

std::string GlobalPreferences::getLanguageIsoCode() const
{
    return d->getLanguageIsoCode();
}

void GlobalPreferences::setLanguageIsoCode(const std::string& languageIsoCode)
{
    d->setLanguageIsoCode(languageIsoCode);
}

std::string GlobalPreferences::getScriptIsoCode() const
{
    return d->getScriptIsoCode();
}

void GlobalPreferences::setScriptIsoCode(const std::string& scriptIsoCode)
{
    d->setScriptIsoCode(scriptIsoCode);
}

std::string GlobalPreferences::getCountryIsoCode() const
{
    return d->getCountryIsoCode();
}

void GlobalPreferences::setCountryIsoCode(const std::string& countryIsoCode)
{
    d->setCountryIsoCode(countryIsoCode);
}

std::string GlobalPreferences::getLocaleName() const
{
    return d->getLocaleName();
}

void GlobalPreferences::setLocaleName(const std::string& localeName)
{
    d->setLocaleName(localeName);
}

}

}
