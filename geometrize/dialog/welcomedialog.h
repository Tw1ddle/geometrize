#pragma once

#include <QDialog>

namespace Ui
{
class WelcomeDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The WelcomeDialog class models the UI for a quickstart tutorial/welcome page shown to users on first launch.
 */
class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget* parent = nullptr);
    ~WelcomeDialog();

public slots:
    void on_closeButton_released();
    void on_videoTutorialsButton_released();

private:
    Ui::WelcomeDialog* ui;
};

}

}
