#pragma once

#include <memory>

#include <QMainWindow>

namespace geometrize
{

namespace job
{
class ImageJob;
}

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
    explicit ImageJobWindow();
    ~ImageJobWindow();

    /**
     * @brief setImageJob Sets the current image job shown by the window.
     * @param job The image job to set on the image job window.
     */
    void setImageJob(job::ImageJob* job);

private slots:
    void on_actionExit_triggered();
    void on_actionLoad_Settings_Template_triggered();
    void on_actionSave_Settings_Template_triggered();
    void on_actionReveal_Launch_Window_triggered();
    void on_runStopButton_clicked();
    void on_stepButton_clicked();
    void on_resetButton_clicked();

private:
    class ImageJobWindowImpl;
    std::unique_ptr<ImageJobWindowImpl> d;
};

}

}
