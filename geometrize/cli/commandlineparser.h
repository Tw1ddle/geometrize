#pragma once

#include <string>
#include <vector>

#include <QStringList>

class QApplication;

namespace geometrize
{

namespace cli
{

/**
 * @brief shouldRunInSelfTestMode Checks whether the application should run in automated self-test/functional testing mode.
 * @param arguments The console arguments.
 * @return True if the application should run in self-test mode, else false.
 */
bool shouldRunInSelfTestMode(const QStringList& arguments);

/**
 * @brief getSelfTestModeScriptDirectories Gets a vector of the directories containing scripts to run in self-test mode
 * @param arguments The console arguments.
 * @return A vector of paths to directories containing scripts to run in self-test mode, empty if there are no tests to run.
 */
std::vector<std::string> getSelfTestModeScriptDirectories(const QStringList& arguments);

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
