#include "commandlineparser.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QString>
#include <QStringList>

#include "chaiscript/chaiscript.hpp"

#include "common/util.h"
#include "job/jobutil.h"
#include "localization/strings.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"

namespace
{
    const QString scriptFlag{"script"};
}

namespace geometrize
{

namespace cli
{

/**
 * @brief shouldRunInConsoleMode Determines whether the Geometrize application should run in console mode.
 * @param arguments The arguments passed to the application on launch.
 * @return True if the application should run in console mode, else false.
 */
bool shouldRunInConsoleMode(const QStringList& arguments)
{
    return arguments.contains(scriptFlag);
}

/**
 * @brief setupCommandLineParser Sets up a command line parser to handle application arguments.
 * @param parser The parser to setup.
 * @param arguments The arguments to parse.
 */
void setupCommandLineParser(QCommandLineParser& parser, const QStringList& arguments)
{
    parser.setApplicationDescription(geometrize::strings::Strings::getApplicationDescription());
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {scriptFlag, QCoreApplication::translate("cli", "Executes the ChaiScript script file at the given path or URL.")}
    });

    if(!parser.parse(arguments)) {
        // TODO throw
        //return CommandLineResult(CommandLineError::CommandLineSetupFailed, parser.errorText());
    }

    parser.process(arguments);
}

/**
 * @brief handleArgumentPairs Handles the arguments that were set on the parser.
 * @param parser The parser to use to use.
 */
void handleCommandLineArguments(QCommandLineParser& parser)
{
    if(parser.isSet(scriptFlag)) {
        const QString scriptPath{parser.value(scriptFlag)};

        const std::string code{geometrize::util::readFileAsString(scriptPath.toStdString())};

        std::unique_ptr<chaiscript::ChaiScript> engine{geometrize::script::createImageJobEngine()};

        geometrize::script::runScript(code, *engine);
    }
}

int runApp(QApplication& app)
{
    const QStringList arguments{app.arguments()};

    QCommandLineParser parser;

    setupCommandLineParser(parser, arguments);
    handleCommandLineArguments(parser);

    return 0;
}

}

}
