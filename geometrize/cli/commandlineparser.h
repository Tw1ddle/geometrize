#pragma once

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
 * @brief runApp Runs the application in console mode.
 * @param app The application.
 * @return 0 on success, any other number if there was an error.
 */
int runApp(QApplication& app);

}

}
