#pragma once

#include <QWidget>

namespace Ui
{
class ScriptConsole;
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

private:
    Ui::ScriptConsole* ui;
};

}

}
