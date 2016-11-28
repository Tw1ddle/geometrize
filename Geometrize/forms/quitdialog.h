#ifndef QUITDIALOG_H
#define QUITDIALOG_H

#include <QDialog>

namespace Ui {
class QuitDialog;
}

class QuitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuitDialog(QWidget *parent = 0);
    ~QuitDialog();

private slots:
    void on_saveButton_clicked();

    void on_dontSaveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::QuitDialog *ui;
};

#endif // QUITDIALOG_H
