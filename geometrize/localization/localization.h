#pragma once

#include <QString>

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

}
