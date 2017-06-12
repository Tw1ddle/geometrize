#pragma once

#include <QGroupBox>

namespace Ui
{
class ScriptEditorWidget;
}

namespace geometrize
{

namespace dialog
{

class ScriptEditorWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ScriptEditorWidget(QWidget* parent = 0);
    ~ScriptEditorWidget();

private:
    Ui::ScriptEditorWidget *ui;
};

}

}
