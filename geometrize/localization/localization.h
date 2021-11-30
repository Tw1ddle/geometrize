#pragma once

#include <QLocale>
#include <QString>
#include <QStringList>

class QIcon;

namespace geometrize
{

/**
 * @brief getBinaryTranslationFileExtension Gets the file extension for Qt binary translation files.
 * @return The file extension for Qt binary file extension, including the dot prefix e.g. ".qm".
 */
QString getBinaryTranslationFileExtension();

/**
 * @brief getAppTranslationResourceDirectory Gets the resource path where translation files specific to the app are stored.
 * @return The resource path where translation files specific to the app are stored.
 */
QString getAppTranslationResourceDirectory();

/**
 * @brief getQtTranslationResourceDirectory Gets the resource path where translation files for Qt itself are stored.
 * @return The resource path where translation files for Qt are stored.
 */
QString getQtTranslationResourceDirectory();

/**
 * @brief installTranslatorsForLocale Installs translators for the application.
 * @param locale A string in the form language_country.
 * Note this is not threadsafe.
 */
void setTranslatorsForLocale(const QString& locale);

/**
 * @brief getFlagIconForLocaleCode Gets a representative national flag for the given locale code.
 * @param localeCode A string in the form language_country_locale.
 * @return An icon for the code, or an error symbol if no suitable icon could be found.
 */
QIcon getFlagIconForLocaleCode(const QString& localeCode);

/**
 * @brief getGlobalPreferencesLocale Gets a QLocale instance based on the current settings in global preferences.
 * Note that if the string violates the locale format, the "C" locale is used instead.
 * @return A QLocale instance based on the global preferences.
 */
QLocale getGlobalPreferencesLocale();

/**
 * @brief setGlobalPreferencesForLocale Sets the locale settings based on the given locale name.
 * @param localeName The locale name.
 * Note this sets only the language and country, not the script or other settings.
 * Even if the string violates the locale format, the string may be saved to the preferences anyway.
 */
void setGlobalPreferencesForLocale(const QLocale& locale);

/**
 * @brief getSupportedLocaleCodes Iterates over the embedded .qm files and extracts the ISO language codes from their filenames
 * @return A list of supported locale codes, sorted alphabetically e.g. "en_GB, ru_RU" etc
 */
QStringList getSupportedLocaleCodes();

/**
 * @brief setLocaleAndUserInterfaceLanguage Sets the application locale and user interface language to the given language code
 * @param isoCode The locale code to set the application to.
 * Note: has a hack that transforms Portuguese "pt" to "pt_PT" to prefer Portuguese over Brazilian-Portuguese
 * Also sets global preferences to remember the given code.
 */
void setLocaleAndUserInterfaceLanguage(const QString& isoCode);

}
