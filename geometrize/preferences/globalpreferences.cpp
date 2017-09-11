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

    bool shouldShowImageJobVectorViewByDefault() const
    {
        return m_shouldShowImageJobVectorViewByDefault;
    }

    void setShouldShowImageJobVectorViewByDefault(const bool showVectorView)
    {
        m_shouldShowImageJobVectorViewByDefault = showVectorView;
    }

    bool shouldShowImageJobConsoleByDefault() const
    {
        return m_shouldShowImageJobConsoleByDefault;
    }

    void setShouldShowImageJobConsoleByDefault(const bool showConsole)
    {
        m_shouldShowImageJobConsoleByDefault = showConsole;
    }

    bool shouldShowImageJobScriptEditorByDefault() const
    {
        return m_shouldShowImageJobScriptEditorByDefault;
    }

    void setShouldShowImageJobScriptEditorByDefault(const bool showScriptEditor)
    {
        m_shouldShowImageJobScriptEditorByDefault = showScriptEditor;
    }

    bool shouldShowImageJobPixmapViewByDefault() const
    {
        return m_shouldShowImageJobPixmapViewByDefault;
    }

    void setShouldShowImageJobPixmapViewByDefault(const bool showPixmapView)
    {
        m_shouldShowImageJobPixmapViewByDefault = showPixmapView;
    }

    void setImageJobResizeThreshold(const std::uint32_t width, const std::uint32_t height)
    {
        m_imageJobResizeThreshold.first = width;
        m_imageJobResizeThreshold.second = height;
    }

    std::pair<std::uint32_t, std::uint32_t> getImageJobResizeThreshold() const
    {
        return m_imageJobResizeThreshold;
    }

    bool isImageJobImageResizeEnabled() const
    {
        return m_imageJobResizeEnabled;
    }

    void setImageJobImageResizeEnabled(const bool enabled)
    {
        m_imageJobResizeEnabled = enabled;
    }

    std::uint32_t getImageJobMaxThreads() const
    {
        return m_imageJobMaxThreads;
    }

    void setImageJobMaxThreads(const std::uint32_t maxThreads)
    {
        m_imageJobMaxThreads = maxThreads;
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
           m_shouldShowImageJobVectorViewByDefault,
           m_shouldShowImageJobConsoleByDefault,
           m_shouldShowImageJobScriptEditorByDefault,
           m_shouldShowImageJobPixmapViewByDefault,

           m_imageJobResizeEnabled,
           m_imageJobResizeThreshold,
           m_imageJobMaxThreads,

           m_languageIsoCode,
           m_scriptIsoCode,
           m_countryIsoCode);
    }

    serialization::GlobalPreferencesData m_data;

    bool m_shouldShowWelcomeScreenOnLaunch{true};

    bool m_shouldGeometrizeAppLogoOnLaunch{true};
    bool m_shouldPopulateRecentItemsOnLaunch{true};
    bool m_shouldPopulateTemplatesOnLaunch{true};

    bool m_shouldShowLaunchConsoleByDefault{false};
    bool m_shouldShowImageJobVectorViewByDefault{true};
    bool m_shouldShowImageJobConsoleByDefault{false};
    bool m_shouldShowImageJobScriptEditorByDefault{false};
    bool m_shouldShowImageJobPixmapViewByDefault{false};

    bool m_imageJobResizeEnabled{false};
    std::pair<std::uint32_t, std::uint32_t> m_imageJobResizeThreshold{512, 512};
    std::uint32_t m_imageJobMaxThreads{0};

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

bool GlobalPreferences::shouldShowImageJobVectorViewByDefault() const
{
    return d->shouldShowImageJobVectorViewByDefault();
}

void GlobalPreferences::setShouldShowImageJobVectorViewByDefault(const bool showVectorView)
{
    d->setShouldShowImageJobVectorViewByDefault(showVectorView);
}

bool GlobalPreferences::shouldShowImageJobConsoleByDefault() const
{
    return d->shouldShowImageJobConsoleByDefault();
}

void GlobalPreferences::setShouldShowImageJobConsoleByDefault(const bool showConsole)
{
    d->setShouldShowImageJobConsoleByDefault(showConsole);
}

bool GlobalPreferences::shouldShowImageJobScriptEditorByDefault() const
{
    return d->shouldShowImageJobScriptEditorByDefault();
}

void GlobalPreferences::setShouldShowImageJobScriptEditorByDefault(const bool showScriptEditor)
{
    d->setShouldShowImageJobScriptEditorByDefault(showScriptEditor);
}

bool GlobalPreferences::shouldShowImageJobPixmapViewByDefault() const
{
    return d->shouldShowImageJobPixmapViewByDefault();
}

void GlobalPreferences::setShouldShowImageJobPixmapViewByDefault(const bool showPixmapView)
{
    d->setShouldShowImageJobPixmapViewByDefault(showPixmapView);
}

bool GlobalPreferences::isImageJobImageResizeEnabled() const
{
    return d->isImageJobImageResizeEnabled();
}

void GlobalPreferences::setImageJobImageResizeEnabled(const bool enabled)
{
    d->setImageJobImageResizeEnabled(enabled);
}

std::pair<std::uint32_t, std::uint32_t> GlobalPreferences::getImageJobResizeThreshold() const
{
    return d->getImageJobResizeThreshold();
}

void GlobalPreferences::setImageJobResizeThreshold(const std::uint32_t width, const std::uint32_t height)
{
    d->setImageJobResizeThreshold(width, height);
}

std::uint32_t GlobalPreferences::getImageJobMaxThreads() const
{
    return d->getImageJobMaxThreads();
}

void GlobalPreferences::setImageJobMaxThreads(const std::uint32_t maxThreads)
{
    d->setImageJobMaxThreads(maxThreads);
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
