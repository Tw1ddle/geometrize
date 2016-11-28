#ifndef PREFERENCESTABDIALOG_H
#define PREFERENCESTABDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesTabDialog;
}

class PreferencesTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesTabDialog(QWidget *parent = 0);
    ~PreferencesTabDialog();

private:
    Ui::PreferencesTabDialog *ui;
};

#endif // PREFERENCESTABDIALOG_H
