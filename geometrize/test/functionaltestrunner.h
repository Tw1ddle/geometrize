#pragma once

#include <QApplication>

#include <string>

namespace geometrize
{

namespace test
{

/**
 * @brief runApp Runs the application in self-test mode.
 * @param app The application.
 * @return 0 on success, any other return code if there was an error running the tests.
 */
int runApp(QApplication& app);

/**
 * @brief runSelfTests Runs the scripts in the given directory, one by one.
 * @param testScriptsDirectory The directory of test scripts to execute.
 */
void runSelfTests(const std::string& testScriptsDirectory);

}

}
