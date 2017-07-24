#include "scriptrunner.h"

#include <QCoreApplication>
#include <QMessageBox>

#include "chaiscript/chaiscript.hpp"

#include "script/chaiscriptcreator.h"

namespace geometrize
{

namespace script
{

void runScript(const std::string& code, chaiscript::ChaiScript& runner)
{
    const QString errorTitle{QCoreApplication::translate("Script evaluation error dialog title", "Script evaluation failure")};
    const QString errorPreamble{QCoreApplication::translate("Script evaluation error message", "Could not evaluate script: %1")};
    const QString unknownError{QCoreApplication::translate("Script evaluation unknown error", "Unknown script evaluation error")};

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

void runScript(const std::string& script)
{
    auto engine{createDefaultEngine()};
    runScript(script, *engine);
}

}

}
