#include "commandlineparser.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QString>
#include <QStringList>

#include "chaiscript/chaiscript.hpp"

#include "common/util.h"
#include "localization/strings.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "task/taskutil.h"

namespace
{
    const QString scriptFileFlag{"script_file"};
    const QString scriptSourceFlag{"script_inline"};
    const QString localeOverrideFlag{"locale_override"};
}

namespace geometrize
{

namespace cli
{

/**
 * @brief setupCommandLineParser Sets up a command line parser to handle application arguments.
 * @param parser The parser to setup.
 * @param arguments The arguments to parse.
 */
void setupCommandLineParser(QCommandLineParser& parser, const QStringList& arguments)
{
    parser.setApplicationDescription(geometrize::strings::Strings::getShortApplicationDescription());
    parser.addHelpOption();
    parser.addVersionOption();

    // NOTE left these untranslated
    parser.addOption(QCommandLineOption(scriptFileFlag, "Executes the ChaiScript script file at the given file path", "File path to ChaiScript script file"));
    parser.addOption(QCommandLineOption(scriptSourceFlag, "Executes the inline ChaiScript source code unmodified", "Inline ChaiScript source code"));
    parser.addOption(QCommandLineOption(localeOverrideFlag, "Overrides the locale and translation that the application launches with", "Locale code"));

    if(!parser.parse(arguments)) {
        assert(0 && "Failed to parse command line arguments");
    }

    parser.process(arguments);
}

/**
 * @brief handleArgumentPairs Handles the arguments that were set on the parser.
 * @param parser The parser to use to use.
 */
void handleCommandLineArguments(QCommandLineParser& parser)
{
    if(parser.isSet(scriptFileFlag)) {
        const QString scriptPath{parser.value(scriptFileFlag)};
        const std::string code{geometrize::util::readFileAsString(scriptPath.toStdString())};

        std::unique_ptr<chaiscript::ChaiScript> engine{geometrize::script::createImageTaskEngine()};
        geometrize::script::runScript(code, *engine);
    } else if(parser.isSet(scriptSourceFlag)) {
        const std::string code{parser.value(scriptSourceFlag).toStdString()};

        std::unique_ptr<chaiscript::ChaiScript> engine{geometrize::script::createImageTaskEngine()};
        geometrize::script::runScript(code, *engine);
    }
}

bool shouldRunInConsoleMode(const QStringList& arguments)
{
    QCommandLineParser parser;
    setupCommandLineParser(parser, arguments);
    return parser.isSet(scriptFileFlag) || parser.isSet(scriptSourceFlag);
}

std::string getOverrideLocaleCode(const QStringList& arguments)
{
    QCommandLineParser parser;
    setupCommandLineParser(parser, arguments);

    if(!parser.isSet(localeOverrideFlag)) {
        return "";
    }
    return parser.value(localeOverrideFlag).toStdString();
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
