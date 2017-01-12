#pragma once

#include <QDialog>

namespace Ui
{
class PreferencesTabDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The PreferencesTabDialog class models the UI for a tabbed preferences dialog for application-level preferences.
 * Note this is a modal dialog, job-specific preferences are edited elsewhere.
 */
class PreferencesTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesTabDialog(QWidget* parent = 0);
    ~PreferencesTabDialog();

private:
    Ui::PreferencesTabDialog* ui;
};

}

}
