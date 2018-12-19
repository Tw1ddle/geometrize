#pragma once

#include <QDialog>

class QEvent;

namespace Ui
{
class CreditsDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CreditsDialog class models the UI for the developer, artwork, localization etc credits pages.
 */
class CreditsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreditsDialog(QWidget* parent = nullptr);
    virtual ~CreditsDialog();

protected:
    void changeEvent(QEvent*) override;

private:
    void populateUi();

    Ui::CreditsDialog* ui;
};

}

}
