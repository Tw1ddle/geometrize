#include "test/functionaltestrunner.h"

#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

#include <QTimer>

#include "chaiscript/chaiscript.hpp"

#include "common/util.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"

namespace
{

std::vector<std::string> scriptDirectories; // Directories containing test scripts
std::vector<std::string> scriptPaths; // Paths to test scripts
std::vector<std::string> completedScriptPaths; // Completed test scripts

bool runNextTest()
{
    if(!geometrize::util::directoriesExist(::scriptDirectories)) {
        assert(0 && "One or more of the test script directories do not exist");
        return false;
    }

    const auto engine = geometrize::script::createFunctionalTestRunnerEngine();

    // Consume the scripts one-by-one, adding the paths to a list of completed scripts (to print when finished)
    if(scriptPaths.empty()) {
        return false;
    }

    const std::string scriptPath = scriptPaths.back();
    scriptPaths.pop_back();
    engine->add_global_const(chaiscript::var(scriptPath), "scriptPath");

    const std::string scriptCode = geometrize::util::readFileAsString(scriptPath);
    if(scriptCode.empty()) {
        assert(0 && "Failed to read script file or it was empty");
        return false;
    }

    geometrize::script::runScript(scriptCode, *engine.get());

    completedScriptPaths.emplace_back(scriptPath);

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
    timer->start(1500);
    QObject::connect(timer, &QTimer::timeout, [] {
        if(scriptPaths.empty()) {
            std::exit(0);
        }
        const bool result = runNextTest();
        if(!scriptPaths.empty() && !result) {
            assert(0 && "Script failed before all of them completed");
            std::exit(-1);
        }
    });
}

}

}
