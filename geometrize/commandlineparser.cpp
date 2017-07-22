#include "commandlineparser.h"

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QString>
#include <QStringList>

#include "strings.h"
#include "job/jobutil.h"
#include "script/scriptrunner.h"
#include "util.h"

namespace
{
    const QString scriptFlag{"script"};
}

namespace geometrize
{

namespace cli
{

CommandLineResult::CommandLineResult(const CommandLineError error, const QString& errorText) : m_error{error}, m_errorText{errorText}
{
}

CommandLineError CommandLineResult::getError() const
{
    return m_error;
}

QString CommandLineResult::getErrorText() const
{
    return m_errorText;
}

CommandLineResult setupCommandLineParser(QCommandLineParser& parser, const QStringList& arguments)
{
    parser.setApplicationDescription(geometrize::strings::Strings::getApplicationDescription());
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"s", scriptFlag}, QCoreApplication::translate("cli", "Executes the ChaiScript script file at the given path or URL.")}
    });

    if(!parser.parse(arguments)) {
        return CommandLineResult(CommandLineError::CommandLineSetupFailed, parser.errorText());
    }

    parser.process(arguments);

    return CommandLineResult(CommandLineError::CommandLineOk, "");
}

CommandLineResult handleArgumentPairs(QCommandLineParser& parser)
{
    if(parser.isSet(scriptFlag)) {
        //Required arg is just the infile for the script, then whatever other args it needs
        // TODO embedded scripts could have their own name?

        // TODO attempt to read and execute script file, all other args will be injected as VAR=value globals
        //geometrize::util::readFileAsString();
        //geometrize::script::runScript();
    }

    return CommandLineResult(CommandLineError::CommandLineOk, "");
}

CommandLineResult handlePositionalArguments(const QStringList& arguments)
{
    return CommandLineResult(CommandLineError::CommandLineOk, ""); // TODO
}

}

}
