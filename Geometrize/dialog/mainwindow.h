#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The MainWindow class models the UI for the main window, a launchpad for opening new job templates and recent jobs.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_emptyImage_Opened();

private:
    Ui::MainWindow *ui;
};

}

}

#endif // MAINWINDOW_H
