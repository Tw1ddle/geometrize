#pragma once

#include <QDialog>

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

private:
    Ui::LicenseDialog* ui;
};

}

}
