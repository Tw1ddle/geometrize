#pragma once

#include <QDialog>

namespace Ui
{
class CreditsDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CreditsDialog class models the UI for the developer, artwork etc credits pages.
 */
class CreditsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreditsDialog(QWidget* parent = 0);
    ~CreditsDialog();

private:
    Ui::CreditsDialog* ui;
};

}

}
