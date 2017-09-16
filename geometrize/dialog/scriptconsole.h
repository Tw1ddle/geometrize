#pragma once

#include <memory>
#include <string>
#include <vector>

#include <QWidget>

class QEvent;

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ScriptConsole class models a command line and output window.
 */
class ScriptConsole : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptConsole(QWidget* parent = nullptr);
    ~ScriptConsole();

    static const std::string launchConsoleHistoryFilename; ///< The name of the console history settings file used by the console embedded in the launch window.

    void setEngine(chaiscript::ChaiScript* engine);
    std::vector<std::string> getHistory() const;
    void setHistory(const std::vector<std::string>& history);

    virtual bool event(QEvent* event) override;

protected:
    void changeEvent(QEvent*) override;

private:
    class ScriptConsoleImpl;
    std::unique_ptr<ScriptConsoleImpl> d;
};

}

}
