#include "scripteditorwidget.h"
#include "ui_scripteditorwidget.h"

#include <cassert>

#include "strings.h"

namespace geometrize
{

namespace dialog
{

class ScriptEditorWidget::ScriptEditorWidgetImpl
{
public:
    ScriptEditorWidgetImpl(ScriptEditorWidget* pQ, const QString& title) : q{pQ}, ui{std::make_unique<Ui::ScriptEditorWidget>()}
    {
        ui->setupUi(q);
        q->setTitle(title);

        // TODO take a chaiscript engine and the name of a function/global variable
        // TODO when apply is clicked, validate and then offer a function back to the shape mutator

        // TODO connect things up
        ui->scriptTextEdit->insertPlainText("TODO");

        q->connect(ui->scriptTextEdit, &QPlainTextEdit::textChanged, []() {

        });

        q->connect(ui->applyScriptButton, &QPushButton::clicked, []() {
            // TODO set the new function in chaiscript...
        });

        q->connect(ui->resetToDefaultButton, &QPushButton::clicked, []() {
            // TODO restore to the function text that was set when the widget was created?
        });
    }

    ScriptEditorWidgetImpl operator=(const ScriptEditorWidgetImpl&) = delete;
    ScriptEditorWidgetImpl(const ScriptEditorWidgetImpl&) = delete;
    ~ScriptEditorWidgetImpl()
    {
    }

private:
    ScriptEditorWidget* q;
    std::unique_ptr<Ui::ScriptEditorWidget> ui;
};

ScriptEditorWidget::ScriptEditorWidget(const QString& title, QWidget* parent) : QGroupBox(parent), d{std::make_unique<ScriptEditorWidgetImpl>(this, title)}
{
}

ScriptEditorWidget::~ScriptEditorWidget()
{
}

}

}
