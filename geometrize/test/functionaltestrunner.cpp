#include "test/functionaltestrunner.h"

#include <cassert>
#include <string>
#include <vector>

#include <QApplication>
#include <QCommandLineParser>

#include "chaiscript/chaiscript.hpp"

#include "cli/commandlineparser.h"
#include "common/util.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"

namespace
{

std::vector<std::string> scriptDirectories;

}

namespace geometrize
{

namespace test
{

void setTestScriptDirectories(const std::vector<std::string>& scriptDirectories)
{
    ::scriptDirectories = scriptDirectories;
}

void addTestScriptDirectory(const std::string& scriptDirectory)
{
    ::scriptDirectories.emplace_back(scriptDirectory);
}

void runSelfTests()
{
    // TODO scan the test script directories and consume the scripts one-by-one, saving the paths to a global object / to print when finished
    // TODO write tests to show all the major dialogs in the application, take screenshots and save them (and setup CI to deploy them to the geometrize_screenshots repo)

    /*
    if(!geometrize::util::directoryExists(testScriptsDirectory)) {
        assert(0 && "Given test scripts directory does not exist");
        return;
    }

    const std::vector<std::string> testScripts{geometrize::util::getScriptsForPath(testScriptsDirectory)};

    if(testScripts.empty()) {
        assert(0 && "Did not find any test scripts in the given test directory");
        return;
    }

    for(const auto& scriptPath : testScripts) {
        const std::string scriptCode = geometrize::util::readFileAsString(scriptPath);
        if(scriptCode.empty()) {
            assert(0 && "Failed to read script file or it was empty");
        }

        const auto engine = geometrize::script::createFunctionalTestRunnerEngine();
        geometrize::script::runScript(scriptCode, *engine.get());
    }
    */
}

}

}
