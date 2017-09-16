#pragma once

#include <QDialog>

class QEvent;

namespace Ui
{
class LicenseDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The LicenseDialog class models the UI for the software license text and notices.
 */
class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenseDialog(QWidget* parent = nullptr);
    ~LicenseDialog();

protected:
    void changeEvent(QEvent*) override;

private:
    void populateUi();

    Ui::LicenseDialog* ui;
};

}

}
