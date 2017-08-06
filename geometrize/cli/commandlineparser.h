#pragma once

#include <string>

#include <QStringList>

class QApplication;

namespace geometrize
{

namespace cli
{

/**
 * @brief shouldRunInConsoleMode Checks whether the application should run in console mode for the given arguments.
 * @param arguments The console arguments.
 * @return True if the application should run in console mode, else false.
 */
bool shouldRunInConsoleMode(const QStringList& arguments);

/**
 * @brief getOverrideLocaleCode Gets the override locale code that the application should start up with.
 * @param arguments The console arguments.
 * @return The override locale code, or an empty string if no override was specified.
 */
std::string getOverrideLocaleCode(const QStringList& arguments);

/**
 * @brief runApp Runs the application in console mode.
 * Note that since Windows does not support dual-mode applications, this isn't "true" console mode - we just don't create a main window/GUI.
 * @param app The application.
 * @return 0 on success, any other return code if there was an error.
 */
int runApp(QApplication& app);

}

}
