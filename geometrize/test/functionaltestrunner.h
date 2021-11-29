#pragma once

#include <QApplication>

#include <string>
#include <vector>

namespace geometrize
{

namespace test
{

/**
 * @brief setTestScriptDirectories Sets the directories containing self-test scripts to run
 * @param scriptDirectories A vector of paths to the directories containing scripts to run scripts
 */
void setTestScriptDirectories(const std::vector<std::string>& scriptDirectories);

/**
 * @brief addTestScriptDirectory Adds a directory containing scripts to run self-tests from
 * @param scriptDirectory The path to the directory containing scripts to run
 */
void addTestScriptDirectory(const std::string& scriptDirectory);

/**
 * @brief runSelfTests Runs the self tests
 * This consumes the tests from the test script directories specified by the call to setTestScriptDirectories
 */
void runSelfTests();

}

}
