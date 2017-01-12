#pragma once

#include <QDialog>

namespace Ui
{
class GlobalPreferencesTabDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The GlobalPreferencesTabDialog class models the UI for a tabbed preferences dialog for application-level preferences.
 * Note this is a modal dialog, job-specific preferences are edited elsewhere.
 */
class GlobalPreferencesTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalPreferencesTabDialog(QWidget* parent = 0);
    ~GlobalPreferencesTabDialog();

private:
    Ui::GlobalPreferencesTabDialog* ui;
};

}

}
