#pragma once

class QApplication;
class QString;

namespace geometrize
{

/**
 * @brief setupLocalization Enables translation for the application.
 * @param application The application to enable translation on.
 */
void setupLocalization(QApplication& application, const QString& locale);

}
