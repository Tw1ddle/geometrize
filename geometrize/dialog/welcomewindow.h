#pragma once

#include <QMainWindow>

class QEvent;

namespace Ui
{
class WelcomeWindow;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The WelcomeWindow class models the UI for a quickstart tutorial/welcome page shown to users on first launch.
 */
class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow();
    virtual ~WelcomeWindow();

protected:
    virtual void closeEvent (QCloseEvent* event) override;
    void changeEvent(QEvent*) override;

public slots:
    void on_closeButton_released();
    void on_videoTutorialsButton_released();
    void on_showOnLaunchCheckbox_clicked(bool checked);

private:
    void populateUi();

    Ui::WelcomeWindow* ui;
};

}

}
