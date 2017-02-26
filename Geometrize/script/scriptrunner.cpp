#include "scriptrunner.h"

#include <QCoreApplication>
#include <QMessageBox>

#include "chaiscript/chaiscript.hpp"
#include "script/chaiscriptcreator.h"
#include "script/scriptoptions.h"

namespace geometrize
{

namespace script
{

void runScript(const std::string& script, chaiscript::ChaiScript& runner, const geometrize::script::ScriptOptions& options)
{
    // TODO set options on the runner?

    const QString errorTitle{QCoreApplication::translate("Script evaluation error dialog title", "Script evaluation failure")};
    const QString errorPreamble{QCoreApplication::translate("Script evaluation error message", "Could not evaluate script: %1")};
    const QString unknownError{QCoreApplication::translate("Script evaluation unknown error", "Unknown script evaluation error")};

    // TODO possibly get rid of message boxes
    try {
        runner.parse(script); // Note parsing first to check the syntax is valid
        runner.eval(script);
    } catch (const std::string& s) {
        QMessageBox::warning(nullptr, errorTitle, errorPreamble.arg(QString::fromStdString(s)));
    } catch (const std::exception& e) {
        QMessageBox::warning(nullptr, errorTitle, errorPreamble.arg(QString::fromStdString(e.what())));
    } catch (...) {
        QMessageBox::warning(nullptr, errorTitle, errorPreamble.arg(unknownError));
    }
}

void runScript(const std::string& script, const ScriptOptions& options)
{
    auto engine{createChaiScript()};
    runScript(script, *engine, options);
}

}

}
