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
 * @brief The ImageJobWindow class encapsulates the UI for manipiulating, running and viewing an image job that geometrizes an image.
 */
class ImageJobWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageJobWindow();
    ~ImageJobWindow();

    /**
     * @brief setImageJob Sets the current image job shown by the window.
     * @param job The image job to set on the image job window. The window takes ownership of the image job.
     */
    void setImageJob(job::ImageJob* job);

signals:
    /**
     * @brief willSwitchImageJob Signal dispatched immediately before an image job is set on the image job window.
     * @param lastJob The last job (if any) set on the window, that will be replaced by the nextJob.
     * @param nextJob The next job (if any) that will be set on the window.
     */
    void willSwitchImageJob(job::ImageJob* lastJob, job::ImageJob* nextJob);

    /**
     * @brief didSwitchImageJob Signal dispatched immediately after an image job is set on the image job window.
     * @param lastJob The last job (if any) set on the window, that was replaced by the nextJob.
     * @param currentJob The job (if any) that was set on the window.
     */
    void didSwitchImageJob(job::ImageJob* lastJob, job::ImageJob* currentJob);

    /**
     * @brief didLoadSettingsTemplate Signal dispatched immediately after a settings template is applied to the image job set on the window.
     */
    void didLoadSettingsTemplate();

    /**
     * @brief didSaveSettingsTemplate Signal dispatched immediately after a settings template based on the current image job settings is saved.
     */
    void didSaveSettingsTemplate();

private slots:
    void on_actionExit_triggered();
    void on_actionLoad_Settings_Template_triggered();
    void on_actionSave_Settings_Template_triggered();
    void on_actionReveal_Launch_Window_triggered();
    void on_actionReveal_Script_Editor_triggered();

private:
    class ImageJobWindowImpl;
    std::unique_ptr<ImageJobWindowImpl> d;
};

}

}
