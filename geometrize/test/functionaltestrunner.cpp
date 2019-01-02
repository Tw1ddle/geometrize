#include "test/functionaltestrunner.h"

#include <cassert>
#include <string>
#include <vector>

#include <QApplication>
#include <QCommandLineParser>

#include "cli/commandlineparser.h"
#include "common/util.h"
#include "script/scriptrunner.h"

namespace geometrize
{

namespace test
{

int runApp(QApplication& app)
{
    return geometrize::cli::runApp(app);
}

void runSelfTests(const std::string& testScriptsDirectory)
{
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
        geometrize::script::runScript(scriptCode);
    }
}

}

}
