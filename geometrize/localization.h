#pragma once

class QCoreApplication;
class QIcon;
class QString;

namespace geometrize
{

/**
 * @brief installTranslatorsForLocale Installs translators for the application.
 * @param application The application to enable translation on.
 * @param locale A string in the form language_country.
 */
void installTranslatorsForLocale(QCoreApplication& application, const QString& locale);

/**
 * @brief getFlagIconForIsoCode Gets a representative national flag for the given ISO 639-1 language code.
 * @param isoCode The ISO 639-1 language code.
 * @return An icon for the code, or an error symbol if no suitable icon could be found.
 */
QIcon getFlagIconForIsoCode(const QString& isoCode);

}
