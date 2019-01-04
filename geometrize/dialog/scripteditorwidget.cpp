#include "scripteditorwidget.h"
#include "ui_scripteditorwidget.h"

#include <cassert>

#include <QEvent>
#include <QFontMetrics>
#include <QLabel>
#include <QSize>

#include "localization/strings.h"

namespace geometrize
{

namespace dialog
{

class ScriptEditorWidget::ScriptEditorWidgetImpl
{
public:
    ScriptEditorWidgetImpl(ScriptEditorWidget* pQ, const std::string& title, const std::string& functionName, const std::string& defaultCode) : q{pQ}, ui{std::make_unique<Ui::ScriptEditorWidget>()}, m_functionName{functionName}, m_defaultCode{defaultCode}
    {
        ui->setupUi(q);
        populateUi();

        q->setTitle(QString::fromStdString(title));

        const QString contents{QString::fromStdString(defaultCode)};
        ui->scriptTextEdit->insertPlainText(contents);

        QFontMetrics metrics(ui->scriptTextEdit->font());
        const QSize size{metrics.size(0, contents)};
        const int padding = 30;
        ui->scriptTextEdit->setMinimumSize(ui->scriptTextEdit->width() + padding, size.height() + padding);

        q->connect(ui->scriptTextEdit, &QPlainTextEdit::textChanged, [this]() {
            setScriptChangesState(ScriptChangesState::UNAPPLIED_CHANGES);

            const std::string code{ui->scriptTextEdit->toPlainText().toStdString()};
            q->signal_scriptChanged(q, m_functionName, code);
        });

        q->connect(ui->resetToDefaultButton, &QPushButton::clicked, [this]() {
            ui->scriptTextEdit->setPlainText(QString::fromStdString(m_defaultCode));

            q->signal_scriptReset(q, m_functionName, m_defaultCode);
        });

        setScriptEvaluationState(ScriptEvaluationState::OK);
        setScriptChangesState(ScriptChangesState::READY);
    }

    ScriptEditorWidgetImpl operator=(const ScriptEditorWidgetImpl&) = delete;
    ScriptEditorWidgetImpl(const ScriptEditorWidgetImpl&) = delete;
    ~ScriptEditorWidgetImpl()
    {
    }

    void resetCodeToDefault()
    {
        ui->scriptTextEdit->clear();
        ui->scriptTextEdit->appendPlainText(QString::fromStdString(m_defaultCode));
    }

    std::string getFunctionName() const
    {
        return m_functionName;
    }

    std::string getDefaultCode() const
    {
        return m_defaultCode;
    }

    std::string getCurrentCode() const
    {
        return ui->scriptTextEdit->toPlainText().toStdString();
    }

    void setCurrentCode(const std::string& code)
    {
        ui->scriptTextEdit->clear();
        ui->scriptTextEdit->appendPlainText(QString::fromStdString(code));
    }

    void onScriptEvaluationSucceeded()
    {
        setScriptEvaluationState(ScriptEvaluationState::OK);
        setScriptChangesState(ScriptChangesState::READY);
        setEvaluationErrorMessage("");
    }

    void onScriptEvaluationFailed(const std::string& errorMessage)
    {
        setScriptEvaluationState(ScriptEvaluationState::ERROR);
        setScriptChangesState(ScriptChangesState::UNAPPLIED_CHANGES);
        setEvaluationErrorMessage(errorMessage);
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {

    }

    enum class ScriptEvaluationState
    {
        OK,
        ERROR
    };

    void setEvaluationErrorMessage(const std::string& message)
    {
        ui->evaluationErrorMessageBox->setText(QString::fromStdString(message));
    }

    void setScriptEvaluationState(const ScriptEvaluationState state)
    {
        switch(state) {
        case ScriptEvaluationState::OK:
            setLabel(ui->scriptEvaluationStateIcon, QPixmap(":/icons/script_green.png"));
            ui->evaluationErrorMessageBox->setStyleSheet("QTextEdit { color: green }");
            ui->evaluationErrorMessageBox->setVisible(false);
            break;
        case ScriptEvaluationState::ERROR:
            setLabel(ui->scriptEvaluationStateIcon, QPixmap(":/icons/script_red.png"));
            ui->evaluationErrorMessageBox->setStyleSheet("QTextEdit { color: red }");
            ui->evaluationErrorMessageBox->setVisible(true);
            break;
        }
    }

    enum class ScriptChangesState
    {
        READY,
        UNAPPLIED_CHANGES
    };

    void setScriptChangesState(const ScriptChangesState state)
    {
        switch(state) {
        case ScriptChangesState::READY:
            setLabel(ui->scriptChangesStateIcon, QPixmap(":/icons/script_go.png"));
            break;
        case ScriptChangesState::UNAPPLIED_CHANGES:
            setLabel(ui->scriptChangesStateIcon, QPixmap(":/icons/script_edit.png"));
            break;
        }
    }

    void setLabel(QLabel* label, QPixmap pixmap)
    {
        label->setPixmap(pixmap);
    }

    ScriptEditorWidget* q;
    std::unique_ptr<Ui::ScriptEditorWidget> ui;
    const std::string m_functionName;
    const std::string m_defaultCode;
};

ScriptEditorWidget::ScriptEditorWidget(const std::string& title, const std::string& functionName, const std::string& defaultCode, QWidget* parent) : QGroupBox(parent), d{std::make_unique<ScriptEditorWidgetImpl>(this, title, functionName, defaultCode)}
{
}

ScriptEditorWidget::~ScriptEditorWidget()
{
}

void ScriptEditorWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QGroupBox::changeEvent(event);
}

void ScriptEditorWidget::resetCodeToDefault()
{
    d->resetCodeToDefault();
}

std::string ScriptEditorWidget::getFunctionName() const
{
    return d->getFunctionName();
}

std::string ScriptEditorWidget::getDefaultCode() const
{
    return d->getDefaultCode();
}

std::string ScriptEditorWidget::getCurrentCode() const
{
    return d->getCurrentCode();
}

void ScriptEditorWidget::setCurrentCode(const std::string& code)
{
    d->setCurrentCode(code);
}

void ScriptEditorWidget::onScriptEvaluationSucceeded()
{
    d->onScriptEvaluationSucceeded();
}

void ScriptEditorWidget::onScriptEvaluationFailed(const std::string& errorMessage)
{
    d->onScriptEvaluationFailed(errorMessage);
}

}

}
