#pragma once

#include <QDialog>

class QEvent;

namespace Ui
{
class RunScriptDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The RunScriptDialog class models the UI for picking scripts to run, and specifying script options.
 */
class RunScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunScriptDialog(QWidget* parent = nullptr);
    virtual ~RunScriptDialog();

    QString getScriptFilepath();

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_scriptFileDialogButton_clicked();
    void on_buttonBox_accepted();

private:
    void populateUi();

    Ui::RunScriptDialog* ui;
};

}

}
