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

void runScript(const std::string& code, chaiscript::ChaiScript& runner, const geometrize::script::ScriptOptions* const options)
{
    // TODO set options on the runner?

    const QString errorTitle{QCoreApplication::translate("Script evaluation error dialog title", "Script evaluation failure")};
    const QString errorPreamble{QCoreApplication::translate("Script evaluation error message", "Could not evaluate script: %1")};
    const QString unknownError{QCoreApplication::translate("Script evaluation unknown error", "Unknown script evaluation error")};

    // TODO possibly get rid of message boxes
    try {
        runner.eval(code);
    } catch (const std::string& s) {
        QMessageBox::warning(nullptr, errorTitle, errorPreamble.arg(QString::fromStdString(s)));
    } catch (const std::exception& e) {
        QMessageBox::warning(nullptr, errorTitle, errorPreamble.arg(QString::fromStdString(e.what())));
    } catch (...) {
        QMessageBox::warning(nullptr, errorTitle, errorPreamble.arg(unknownError));
    }
}

void runScript(const std::string& script, const ScriptOptions* const options)
{
    auto engine{createDefaultEngine()};
    runScript(script, *engine, options);
}

}

}
