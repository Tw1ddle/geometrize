#include "test/functionaltestrunner.h"

#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

#include <QApplication>
#include <QTimer>

#include "chaiscript/chaiscript.hpp"

#include "common/util.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"

namespace
{

std::vector<std::string> scriptDirectories; // Directories containing test scripts
std::vector<std::string> scriptPaths; // Paths to test scripts

bool runNextTest()
{
    if(!geometrize::util::directoriesExist(::scriptDirectories)) {
        assert(0 && "One or more of the test script directories do not exist");
        return false;
    }

    const auto engine = geometrize::script::createFunctionalTestRunnerEngine();

    // Consume the scripts one-by-one
    if(scriptPaths.empty()) {
        return false;
    }

    const std::string scriptPath = scriptPaths.back();
    scriptPaths.pop_back();

    // Set some globals that are used across several functional test scripts
    engine->add_global(chaiscript::var(scriptPath), "scriptPath");

    const std::string scriptDirectory = geometrize::util::getDirectoryForFilePath(scriptPath);
    engine->add_global(chaiscript::var(scriptDirectory), "scriptDirectory");

    const std::string screengrabResultsDirectory = scriptDirectory + "/../results/screengrabs";
    engine->add_global(chaiscript::var(screengrabResultsDirectory), "screengrabResultsDirectory");

    const std::string scriptCode = geometrize::util::readFileAsString(scriptPath);
    if(scriptCode.empty()) {
        assert(0 && "Failed to read script file or it was empty");
        return false;
    }

    geometrize::script::runScript(scriptCode, *engine.get());

    return true;
}

}

namespace geometrize
{

namespace test
{

void setTestScriptDirectories(const std::vector<std::string>& scriptDirectories)
{
    ::scriptDirectories = scriptDirectories;
    ::scriptPaths = geometrize::util::getScriptsForPaths(::scriptDirectories);
}

void addTestScriptDirectory(const std::string& scriptDirectory)
{
    ::scriptDirectories.emplace_back(scriptDirectory);
    ::scriptPaths = geometrize::util::getScriptsForPaths(::scriptDirectories);
}

void runSelfTests()
{
    QTimer* timer = new QTimer(QApplication::instance());
    timer->start(2500);
    QObject::connect(timer, &QTimer::timeout, [] {
        if(scriptPaths.empty()) {
            QApplication::exit(0);
        }
        const bool result = runNextTest();
        if(!scriptPaths.empty() && !result) {
            assert(0 && "Functional test script failed to run");
            QApplication::exit(-1);
        }
    });
}

}

}
