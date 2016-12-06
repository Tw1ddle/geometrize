#ifndef LAUNCHWINDOW_H
#define LAUNCHWINDOW_H

#include <QMainWindow>

namespace Ui {
class LaunchWindow;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The LaunchWindow class models the UI for a launchpad for opening new job templates and recent jobs.
 */
class LaunchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LaunchWindow(QWidget *parent = 0);
    ~LaunchWindow();

private slots:
    void on_emptyImage_Opened();
    void on_actionPreferences_triggered();
    void on_actionClear_Recents_triggered();
    void on_actionExit_triggered();
    void on_actionTutorials_triggered();
    void on_actionSupport_triggered();
    void on_actionAbout_triggered();

private:
    void closeEvent(QCloseEvent *bar) override;

    Ui::LaunchWindow *ui;
};

}

}

#endif // LAUNCHWINDOW_H
