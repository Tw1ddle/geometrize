#ifndef IMAGEJOBWINDOW_H
#define IMAGEJOBWINDOW_H

#include <QMainWindow>

namespace Ui {
class ImageJobWindow;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobWindow class models the UI for an image job.
 * An image job contains all the data needed for geometrizing an image.
 */
class ImageJobWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageJobWindow(QWidget *parent = 0);
    ~ImageJobWindow();

private slots:
    void on_imageJob_updated();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionTechnical_Support_triggered();
    void on_actionOnline_Tutorials_triggered();
    void on_actionOpen_New_Image_triggered();
    void on_actionSave_Geometrized_Image_triggered();
    void on_actionOpenPreferences_triggered();
    void on_runStopButton_clicked();

private:
    Ui::ImageJobWindow *ui;

    int exitApplication();

    void closeEvent(QCloseEvent *bar) override;
};

}

}

#endif // IMAGEJOBWINDOW_H
