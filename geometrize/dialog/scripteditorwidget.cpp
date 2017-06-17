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
    ScriptEditorWidgetImpl(ScriptEditorWidget* pQ, const QString& title, const std::string& targetName, const std::string& defaultCode) : q{pQ}, m_targetName{targetName}, m_defaultCode{defaultCode}, ui{std::make_unique<Ui::ScriptEditorWidget>()}
    {
        ui->setupUi(q);
        q->setTitle(title);

        ui->scriptTextEdit->insertPlainText(QString::fromStdString(defaultCode));

        q->connect(ui->scriptTextEdit, &QPlainTextEdit::textChanged, [this]() {
            emit signal_scriptCodeChanged(q, m_targetName, m_defaultCode);
        });

        q->connect(ui->applyScriptButton, &QPushButton::clicked, [this]() {
            const std::string code{ui->scriptTextEdit->toPlainText().toStdString()};
            emit signal_scriptCommitted(q, m_targetName, code);
        });

        q->connect(ui->resetToDefaultButton, &QPushButton::clicked, [this]() {
            ui->scriptTextEdit->setPlainText(QString::fromStdString(m_defaultCode));
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
    const std::string m_targetName;
    const std::string m_defaultCode;
};

ScriptEditorWidget::ScriptEditorWidget(const QString& title, const std::string& targetName, const std::string& defaultCode, QWidget* parent) : QGroupBox(parent), d{std::make_unique<ScriptEditorWidgetImpl>(this, title, targetName, defaultCode)}
{
}

ScriptEditorWidget::~ScriptEditorWidget()
{
}

}

}
