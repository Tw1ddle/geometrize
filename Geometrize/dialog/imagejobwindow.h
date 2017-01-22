#pragma once

#include <memory>

#include <QMainWindow>

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
public:
    explicit ImageJobWindow(QWidget* parent = nullptr);
    ~ImageJobWindow();

private slots:
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionTechnical_Support_triggered();
    void on_actionOnline_Tutorials_triggered();
    void on_actionOpenPreferences_triggered();
    void on_runStopButton_clicked();

private:
    void closeEvent(QCloseEvent* bar) override;

    class ImageJobWindowImpl;
    std::unique_ptr<ImageJobWindowImpl> d;
};

}

}
