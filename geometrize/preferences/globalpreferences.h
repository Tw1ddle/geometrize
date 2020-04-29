#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include <QObject>

namespace geometrize
{

namespace preferences
{
class GlobalPreferences;
}

}

namespace geometrize
{

namespace preferences
{

/**
 * @brief getGlobalPreferences Shorthand function that gets a reference to the shared global preferences object, which stores global application preferences.
 * @return The global preferences object.
 */
geometrize::preferences::GlobalPreferences& getGlobalPreferences();

/**
 * @brief getGlobalPreferencesConfigPath Gets the absolute path to where the global preferences config file should be read/written.
 * @return The absolute path to the global preferences config file.
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
    GlobalPreferences();
    GlobalPreferences& operator=(const GlobalPreferences&) = delete;
    GlobalPreferences(const GlobalPreferences&) = delete;
    ~GlobalPreferences();

    /**
     * @brief load Loads the global preferences from a file.
     * @param filePath The path to the global preferences file.
     * @return True if loading succeeded, else false.
     */
    bool load(const std::string& filePath);

    /**
     * @brief save Saves the global preferences to a file.
     * @param filePath The path to the global preferences file.
     * @return True if saving succeeded, else false.
     */
    bool save(const std::string& filePath);

    /**
     * @brief getApplicationLaunchCount
     * @return Gets the number of times the application has been launched.
     */
    unsigned int getApplicationLaunchCount() const;

    /**
     * @brief incrementApplicationLaunchCount Increments the count for the number of times the application has been launched.
     */
    void incrementApplicationLaunchCount();

    /**
     * @brief shouldShowWelcomeScreenOnLaunch Gets the preference for whether the welcome screen should be displayed on launch.
     * @return True if the welcome screen should be shown on launch.
     */
    bool shouldShowWelcomeScreenOnLaunch() const;

    /**
     * @brief setShouldShowWelcomeScreenOnLaunch Sets the preference for whether the welcome screen should be displayed on launch.
     * @param show Whether to show the welcome screen on launch.
     */
    void setShouldShowWelcomeScreenOnLaunch(bool show);

    /**
     * @brief shouldGeometrizeAppLogoOnLaunch Gets the preference for whether the app logo on the launch window should be geometrized on launch.
     * @return Whether the app logo on the launch window should be geometrized on launch.
     */
    bool shouldGeometrizeAppLogoOnLaunch() const;

    /**
     * @brief setShouldGeometrizeAppLogoOnLaunch Sets the preference for whether to geometrize the app logo on the launch window on launch.
     * @param geometrizeLogo Whether to geometrize the app logo on the launch window on launch.
     */
    void setShouldGeometrizeAppLogoOnLaunch(bool geometrizeLogo);

    /**
     * @brief shouldPopulateRecentItemsOnLaunch Gets the preference for whether the recent items list should be populated on launch.
     * @return Whether to populate the recent items list on launch.
     */
    bool shouldPopulateRecentItemsOnLaunch() const;

    /**
     * @brief setShouldPopulateRecentItemsOnLaunch Sets the preference for whether the recent items list should be populated on launch.
     * @param populateRecents Whether to populate the recent items list on launch.
     */
    void setShouldPopulateRecentItemsOnLaunch(bool populateRecents);

    /**
     * @brief shouldPopulateTemplatesOnLaunch Gets the preference for whether to populate the task templates on the launch window on launch.
     * @return Whether to populate the task templates on the launch window on launch.
     */
    bool shouldPopulateTemplatesOnLaunch() const;

    /**
     * @brief setShouldPopulateTemplatesOnLaunch Sets the preference for whether to populate the task templates on the launch window on launch.
     * @param populateTemplates Whether to populate the task templates on the launch window on app launch.
     */
    void setShouldPopulateTemplatesOnLaunch(bool populateTemplates);

    /**
     * @brief getCustomImageTaskBackgroundOverrideColor Gets the preference for the custom image task background override color to use when an image task is initially set up.
     * @return An array containing the RGBA override color values.
     */
    std::array<std::int32_t, 4> getCustomImageTaskBackgroundOverrideColor() const;

    /**
     * @brief setUseCustomImageTaskBackgroundOverrideColor Sets the preference for the custom image task background override color to use when an image task is initially set up.
     * @param r The red color component
     * @param g The green color component
     * @param b The blue color component
     * @param a The alpha color component
     */
    void setCustomImageTaskBackgroundOverrideColor(std::int32_t r, std::int32_t g, std::int32_t b, std::int32_t a);

    /**
     * @brief shouldUseCustomImageTaskBackgroundOverrideColor Gets the preference for whether to use the custom image task background override color.
     * @return Whether to use the custom image task background override color when setting up image tasks etc.
     */
    bool shouldUseCustomImageTaskBackgroundOverrideColor() const;

    /**
     * @brief setShouldUseCustomImageTaskBackgroundOverrideColor Sets the preference for whether to use the custom image task background override color.
     * @param useColor Whether to use the custom image task background override color when setting up image tasks etc.
     */
    void setShouldUseCustomImageTaskBackgroundOverrideColor(bool useColor);

    /**
     * @brief getTargetImageAlphaReplacementColor Gets the preference for the color to replace the alpha of a target image with, when an image task is initially set up.
     * @return An array containing the RGBA target image alpha replacement color.
     */
    std::array<std::int32_t, 4> getTargetImageAlphaReplacementColor() const;

    /**
     * @brief setTargetImageAlphaReplacementColor Sets the preference for the color to replace the alpha of a target image with, when an image task is initially set up.
     * @param r The red color component
     * @param g The green color component
     * @param b The blue color component
     * @param a The alpha color component
     */
    void setTargetImageAlphaReplacementColor(std::int32_t r, std::int32_t g, std::int32_t b, std::int32_t a);

    /**
     * @brief shouldUseImageAlphaReplacementColor Gets the preference for whether to replace the alpha of a target image with a color, when an image task is initially set up.
     * @return Whether to replace the alpha of a target image with a color, when an image task is initially set up etc.
     */
    bool shouldUseImageAlphaReplacementColor() const;

    /**
     * @brief setShouldReplaceTargetImageAlphaWithCustomColor Sets the preference for whether to replace the alpha of a target image with a color, when an image task is initially set up.
     * @param useColor Whether to replace the alpha of a target image with a color, when an image task is initially set up etc.
     */
    void setShouldReplaceTargetImageAlphaWithCustomColor(bool useColor);

    /**
     * @brief shouldAutoLoadImageTaskSettings Gets the preference for whether to automatically load any previously saved image task settings when a task window is opened.
     * @return Whether to automatically load any previously saved image task settings
     */
    bool shouldAutoLoadImageTaskSettings() const;

    /**
     * @brief setShouldAutoLoadImageTaskSettings Sets the preference for whether to automatically load any previously saved image task settings when a task window is opened.
     * @param loadSettings Whether to automatically load any previously saved image task settings when a task window is opened
     */
    void setShouldAutoLoadImageTaskSettings(bool loadSettings);

    /**
     * @brief shouldAutoSaveImageTaskSettings Gets the preference for whether to automatically save the last task settings to disk/persistent storage when a task window is closed.
     * @return Whether to automatically save the last task settings to disk/persistent storage when a task window is closed
     */
    bool shouldAutoSaveImageTaskSettings() const;

    /**
     * @brief setShouldAutoSaveImageTaskSettings Sets the preference for whether to automatically save the last task settings to disk/persistent storage when a task window is closed.
     * @param saveSettings Whether to automatically save the last task settings to disk/persistent storage when a task window is closed
     */
    void setShouldAutoSaveImageTaskSettings(bool saveSettings);

    /**
     * @brief shouldShowLaunchConsoleByDefault Gets the preference for whether to show the console on the launch window when the window is opened.
     * @return Whether to show the console on the launch window when the window is opened.
     */
    bool shouldShowLaunchConsoleByDefault() const;

    /**
     * @brief setShouldShowLaunchConsoleByDefault Sets the preference for whether to show the console on the launch window when the window is opened.
     * @param showConsole Whether to show the show the console on the launch window when the window is opened.
     */
    void setShouldShowLaunchConsoleByDefault(bool showConsole);

    /**
     * @brief shouldShowImageTaskVectorViewByDefault Gets the preference for whether to show the image task vector/SVG view when the window is opened.
     * @return Whether to show the image task vector/SVG view when the window is opened.
     */
    bool shouldShowImageTaskVectorViewByDefault() const;

    /**
     * @brief setShouldShowImageTaskVectorViewByDefault Sets the preference for whether to show the image task vector/SVG view when the window is opened.
     * @param showVectorView Whether to show the image task vector/SVG view when the window is opened.
     */
    void setShouldShowImageTaskVectorViewByDefault(bool showVectorView);

    /**
     * @brief shouldShowImageTaskConsoleByDefault Gets the preference for whether to show the console on the image task window when the window is opened.
     * @return Whether to show the console on the image task window when the window is opened.
     */
    bool shouldShowImageTaskConsoleByDefault() const;

    /**
     * @brief setShouldShowImageTaskConsoleByDefault Sets the preference for whether to show the console on the image task window when the window is opened.
     * @param showConsole Whether to show the console on the image task window when the window is opened.
     */
    void setShouldShowImageTaskConsoleByDefault(bool showConsole);

    /**
     * @brief shouldShowImageTaskScriptEditorByDefault Gets the preference for whether to open the script editor when an image task window is opened.
     * @return Whether to open the script editor when the image task window is opened.
     */
    bool shouldShowImageTaskScriptEditorByDefault() const;

    /**
     * @brief setShouldShowImageTaskScriptEditorByDefault Sets the preference for whether to open the script editor when an image task window is opened.
     * @param showScriptEditor Whether to open the script editor when an image task window is opened.
     */
    void setShouldShowImageTaskScriptEditorByDefault(bool showScriptEditor);

    /**
     * @brief shouldShowImageTaskPixmapViewByDefault Gets the preference for whether to show the image task pixmap/raster view when the window is opened.
     * @return Whether to show the image task pixmap/raster view when the window is opened.
     */
    bool shouldShowImageTaskPixmapViewByDefault() const;

    /**
     * @brief setShouldShowImageTaskPixmapViewByDefault Sets the preference for whether to show the image task pixmap/raster view when the window is opened.
     * @param showPixmapView Whether to show the image task pixmap/raster view when the window is opened.
     */
    void setShouldShowImageTaskPixmapViewByDefault(bool showPixmapView);

    /**
     * @brief isImageTaskImageResizeEnabled Gets the preference for whether image tasks should resize images that are too large.
     * @return True if the image task image resizing is enabled.
     */
    bool isImageTaskImageResizeEnabled() const;

    /**
     * @brief setImageTaskImageResizeEnabled Sets the preference for whether image tasks should resize images that are too large.
     * @param enabled Whether to enable or disable the resizing.
     */
    void setImageTaskImageResizeEnabled(bool enabled);

    /**
     * @brief getImageTaskResizeThreshold Gets the maximum dimensions of an image that can be used in an image task without resizing.
     * @return The maximum width and height of the image task image that can be used without the image being resized.
     */
    std::pair<std::uint32_t, std::uint32_t> getImageTaskResizeThreshold() const;

    /**
     * @brief setImageTaskResizeThreshold Sets the maximum dimensions of an image that can be used in an image task without resizing.
     * @param width The maximum width of an image.
     * @param height The maximum height of an image.
     */
    void setImageTaskResizeThreshold(std::uint32_t width, std::uint32_t height);

    /**
     * @brief getImageTaskMaxThreads Gets the maximum number of threads the image task should use.
     * @return The maximum number of threads the image task should use.
     */
    std::uint32_t getImageTaskMaxThreads() const;

    /**
     * @brief setImageTaskMaxThreads Sets the maximum number of threads the image task should use.
     * @param maxThreads The maximum number of threads the image task should use.
     */
    void setImageTaskMaxThreads(std::uint32_t maxThreads);

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
     * @brief getScriptIsoCode Gets the ISO-15924 script code preference for the application.
     * @return The ISO-15924 script code.
     */
    std::string getScriptIsoCode() const;

    /**
     * @brief setScriptIsoCode Sets the ISO-15924 script code preference for the application.
     * @param scriptIsoCode The ISO-15924 script code.
     */
    void setScriptIsoCode(const std::string& scriptIsoCode);

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

    /**
     * @brief getLocaleName Gets the locale name in the format "language[_script][_country]".
     * language is a lowercase, two or three letter, ISO 639 language code.
     * script is a titlecase, four-letter, ISO 15924 script code.
     * country is an uppercase, two-letter, ISO 3166 country code (also "419" as defined by United Nations).
     * @return The locale name.
     */
    std::string getLocaleName() const;

    /**
     * @brief setLocaleName Sets the locale with the specified name, which must have the format "language[_script][_country]".
     * @param localeName The locale name in the format "language[_script][_country]".
     * language is a lowercase, two or three letter, ISO 639 language code.
     * script is a titlecase, four-letter, ISO 15924 script code.
     * country is an uppercase, two-letter, ISO 3166 country code (also "419" as defined by United Nations).
     */
    void setLocaleName(const std::string& localeName);

private:
    class GlobalPreferencesImpl;
    std::unique_ptr<GlobalPreferencesImpl> d;
};

}

}
