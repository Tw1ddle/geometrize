#pragma once

#include <QString>
#include <QStringList>

class QCommandLineParser;

namespace geometrize
{

namespace cli
{

namespace options
{
    const QString script{"script"};
    const QString eval{"eval"};
}

enum class CommandLineError
{
    CommandLineOk,
    CommandLineSetupFailed
};

class CommandLineResult
{
public:
    CommandLineResult(CommandLineError error, const QString& errorText);

    CommandLineError getError() const;
    QString getErrorText() const;

private:
    CommandLineError m_error;
    QString m_errorText;
};

/**
 * @brief setupCommandLineParser Sets up a command line parser to handle application arguments.
 * @param parser The parser to setup.
 * @param arguments The arguments to parse.
 * @return The result of the initial setup and parsing.
 */
CommandLineResult setupCommandLineParser(QCommandLineParser& parser, const QStringList& arguments);

/**
 * @brief handleArgumentPairs Handles the argument-value pairs that were set on the parser.
 * @param parser The parser to use to use.
 * @return  The result of handling the argument-value pairs.
 */
CommandLineResult handleArgumentPairs(QCommandLineParser& parser);

/**
 * @brief handlePositionalArguments
 * @param arguments The positional arguments to handle.
 * @return The result of handling the positional arguments.
 */
CommandLineResult handlePositionalArguments(const QStringList& arguments);

}

}
