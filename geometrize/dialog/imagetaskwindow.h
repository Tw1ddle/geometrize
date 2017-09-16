#pragma once

#include <memory>

#include <QMainWindow>

class QEvent;

namespace geometrize
{

namespace task
{
class ImageTask;
}

}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskWindow class encapsulates the UI for manipiulating, running and viewing an image task that geometrizes an image.
 */
class ImageTaskWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageTaskWindow();
    ~ImageTaskWindow();

    /**
     * @brief setImageTask Sets the current image task shown by the window.
     * @param task The image task to set on the image task window. The window takes ownership of the image task.
     */
    void setImageTask(task::ImageTask* task);

signals:
    /**
     * @brief willSwitchImageTask Signal dispatched immediately before an image task is set on the image task window.
     * @param lastTask The last task (if any) set on the window, that will be replaced by the nextTask.
     * @param nextTask The next task (if any) that will be set on the window.
     */
    void willSwitchImageTask(task::ImageTask* lastTask, task::ImageTask* nextTask);

    /**
     * @brief didSwitchImageTask Signal dispatched immediately after an image task is set on the image task window.
     * @param lastTask The last task (if any) set on the window, that was replaced by the nextTask.
     * @param currentTask The task (if any) that was set on the window.
     */
    void didSwitchImageTask(task::ImageTask* lastTask, task::ImageTask* currentTask);

    /**
     * @brief didLoadSettingsTemplate Signal dispatched immediately after a settings template is applied to the image task set on the window.
     */
    void didLoadSettingsTemplate();

    /**
     * @brief didSaveSettingsTemplate Signal dispatched immediately after a settings template based on the current image task settings is saved.
     */
    void didSaveSettingsTemplate();

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_actionExit_triggered();
    void on_actionLoad_Settings_Template_triggered();
    void on_actionSave_Settings_Template_triggered();
    void on_actionReveal_Launch_Window_triggered();
    void on_actionReveal_Script_Editor_triggered();

private:
    class ImageTaskWindowImpl;
    std::unique_ptr<ImageTaskWindowImpl> d;
};

}

}
