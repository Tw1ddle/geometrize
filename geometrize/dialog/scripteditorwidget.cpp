#include "scripteditorwidget.h"
#include "ui_scripteditorwidget.h"

namespace geometrize
{

namespace dialog
{

ScriptEditorWidget::ScriptEditorWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ScriptEditorWidget)
{
    ui->setupUi(this);
}

ScriptEditorWidget::~ScriptEditorWidget()
{
    delete ui;
}

}

}
