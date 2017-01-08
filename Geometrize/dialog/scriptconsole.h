#pragma once

#include <memory>

#include <QWidget>

namespace Ui
{
class ScriptConsole;
}

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ScriptConsole class models a console-style input
 */
class ScriptConsole : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptConsole(QWidget* parent = 0);
    ~ScriptConsole();

    void setEngine(chaiscript::ChaiScript* engine);

private:
    Ui::ScriptConsole* ui;

    class ScriptConsoleImpl;
    std::unique_ptr<ScriptConsoleImpl> d;
};

}

}
