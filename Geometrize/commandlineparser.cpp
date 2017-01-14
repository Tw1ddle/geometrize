#include "commandlineparser.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QString>
#include <QStringList>

#include "constants.h"

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
    parser.setApplicationDescription(geometrize::constants::Strings::getApplicationDescription());
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"s", options::script}, QCoreApplication::translate("cli", "Executes the ChaiScript script file at the given path or URL.")},
        {{"e", options::eval}, QCoreApplication::translate("cli", "Evaluates the ChaiScript input provided directly.")}
    });

    if(!parser.parse(arguments)) {
        return CommandLineResult(CommandLineError::CommandLineSetupFailed, parser.errorText());
    }

    parser.process(arguments);

    return CommandLineResult(CommandLineError::CommandLineOk, "");
}

CommandLineResult handlePositionalArguments(const QStringList& arguments)
{
    for(const QString& argument : arguments) {
        // TODO validate and open jobs, else error
    }

    return CommandLineResult(CommandLineError::CommandLineOk, ""); // TODO
}

}

}
