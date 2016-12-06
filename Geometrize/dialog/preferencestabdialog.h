#ifndef PREFERENCESTABDIALOG_H
#define PREFERENCESTABDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesTabDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The PreferencesTabDialog class models the UI for the tabbed preferences dialog with various application settings.
 * Note this is a modal dialog, job-specific preferences are edited in a dialog.
 */
class PreferencesTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesTabDialog(QWidget *parent = 0);
    ~PreferencesTabDialog();

private:
    Ui::PreferencesTabDialog *ui;
};

}

}

#endif // PREFERENCESTABDIALOG_H
