#include "globalpreferences.h"

#include <assert.h>
#include <fstream>
#include <ostream>

#include <QCoreApplication>

#include "serialization/globalpreferencesdata.h"
#include "cereal/archives/json.hpp"

namespace geometrize
{

namespace preferences
{

std::string getGlobalPreferencesConfigPath()
{
    return QCoreApplication::applicationDirPath().toStdString().append("/config/global_preferences.json");
}

class GlobalPreferences::GlobalPreferencesImpl
{
public:
    GlobalPreferencesImpl(const std::string& filePath)
    {
        load(filePath);
    }
    ~GlobalPreferencesImpl() = default;
    GlobalPreferencesImpl& operator=(const GlobalPreferencesImpl&) = default;
    GlobalPreferencesImpl(const GlobalPreferencesImpl&) = default;

    void load(const std::string& filePath)
    {
        std::ifstream input(filePath);
        try {
            m_data.archive(cereal::JSONInputArchive(input), m_imageJobResizeEnabled, m_imageJobResizeThreshold, m_languageIsoCode);
        } catch(...) {
            assert(0 && "Failed to read global preferences");
        }
    }

    void save(const std::string& filePath)
    {
        std::ofstream output(filePath);
        try {
            m_data.archive(cereal::JSONOutputArchive(output), m_imageJobResizeEnabled, m_imageJobResizeThreshold, m_languageIsoCode);
        } catch(...) {
            assert(0 && "Failed to write global preferences");
        }
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

    std::string getLanguageIsoCode() const
    {
        return m_languageIsoCode;
    }

    void setLanguageIsoCode(const std::string& languageIsoCode)
    {
        m_languageIsoCode = languageIsoCode;
    }

private:
    serialization::GlobalPreferencesData m_data;

    bool m_imageJobResizeEnabled{false};
    std::pair<std::uint32_t, std::uint32_t> m_imageJobResizeThreshold{512, 512};
    std::string m_languageIsoCode{"en"};
};

GlobalPreferences::GlobalPreferences(const std::string& preferencesFilePath) : d{std::make_unique<GlobalPreferences::GlobalPreferencesImpl>(preferencesFilePath)}
{
}

GlobalPreferences::~GlobalPreferences()
{
}

void GlobalPreferences::load(const std::string& filePath)
{
    d->load(filePath);
}

void GlobalPreferences::save(const std::string& filePath)
{
    d->save(filePath);
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

std::string GlobalPreferences::getLanguageIsoCode() const
{
    return d->getLanguageIsoCode();
}

void GlobalPreferences::setLanguageIsoCode(const std::string& languageIsoCode)
{
    d->setLanguageIsoCode(languageIsoCode);
}

}

}
