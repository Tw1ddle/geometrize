#pragma once

#include <QDialog>

namespace Ui
{
class QuitDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The QuitDialog class models the UI of the challenge dialog when the user tries to quit the application.
 */
class QuitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuitDialog(QWidget* parent = 0);
    ~QuitDialog();

private slots:
    void on_saveButton_clicked();
    void on_dontSaveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::QuitDialog* ui;
};

}

}
