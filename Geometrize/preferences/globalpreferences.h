#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include <QObject>

namespace geometrize
{

namespace preferences
{

/**
 * @brief getGlobalPreferencesConfigPath Gets the absolute path to the global preferences config file.
 * @return A path to the global preferences config file.
 */
std::string getGlobalPreferencesConfigPath();

/**
 * @brief The GlobalPreferences class models the preferences associated with the application as a whole.
 * These mostly represent application-wide settings that are edited via the tabbed preferences widget.
 */
class GlobalPreferences : public QObject
{
    Q_OBJECT
public:
    GlobalPreferences(const std::string& preferencesFilePath);
    GlobalPreferences& operator=(const GlobalPreferences&) = delete;
    GlobalPreferences(const GlobalPreferences&) = delete;
    ~GlobalPreferences();

    /**
     * @brief load Loads the global preferences from a file.
     * @param filePath The path to the global preferences file.
     */
    void load(const std::string& filePath);

    /**
     * @brief save Saves the global preferences to a file.
     * @param filePath The path to the global preferences file.
     */
    void save(const std::string& filePath);

    /**
     * @brief isImageJobImageResizeEnabled Gets the preference for whether image jobs should resize images that are too large.
     * @return True if the image job image resizing is enabled.
     */
    bool isImageJobImageResizeEnabled() const;

    /**
     * @brief setImageJobImageResizeEnabled Sets the preference for whether image jobs should resize images that are too large.
     * @param enabled Whether to enable or disable the resizing.
     */
    void setImageJobImageResizeEnabled(bool enabled);

    /**
     * @brief getImageJobResizeThreshold Gets the maximum dimensions of an image that can be used in an image job without resizing.
     * @return The maximum width and height of the image job image that can be used without the image being resized.
     */
    std::pair<std::uint32_t, std::uint32_t> getImageJobResizeThreshold() const;

    /**
     * @brief setImageJobResizeThreshold Sets the maximum dimensions of an image that can be used in an image job without resizing.
     * @param width The maximum width of an image.
     * @param height The maximum height of an image.
     */
    void setImageJobResizeThreshold(std::uint32_t width, std::uint32_t height);

    /**
     * @brief getLanguageIsoCode Gets the ISO 639-1 language preference for the application.
     * @return The ISO 639-1 language code.
     */
    std::string getLanguageIsoCode() const;

    /**
     * @brief setLanguageIsoCode Sets the ISO 639-1 language preference for the application.
     * @param languageIsoCode The ISO 639-1 language code.
     */
    void setLanguageIsoCode(const std::string& languageIsoCode);

    /**
     * @brief getCountryIsoCode Gets the ISO-3166 country code preference for the application.
     * @return The ISO-3166 country code.
     */
    std::string getCountryIsoCode() const;

    /**
     * @brief setCountryIsoCode Sets the ISO-3166 country code preference for the application.
     * @param countryIsoCode The ISO-3166 country code.
     */
    void setCountryIsoCode(const std::string& countryIsoCode);

private:
    class GlobalPreferencesImpl;
    std::unique_ptr<GlobalPreferencesImpl> d;
};

}

}
