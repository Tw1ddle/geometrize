#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_imageJob_updated();

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_actionTechnical_Support_triggered();

    void on_actionOnline_Tutorials_triggered();

    void on_actionOpen_New_Image_triggered();

    void on_actionSave_Geometrized_Image_triggered();

private:
    Ui::MainWindow *ui;

    int exitApplication();

    void closeEvent(QCloseEvent *bar) override;
};

#endif // MAINWINDOW_H
