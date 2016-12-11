#pragma once

#include <QDialog>

namespace Ui {
class AboutDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The AboutDialog class models the UI for the vendor info, license details, credits etc.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

public slots:
    void on_closeButton_released();
    void on_licenseButton_released();
    void on_creditsButton_released();

private:
    Ui::AboutDialog *ui;
};

}

}
