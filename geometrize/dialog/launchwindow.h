#pragma once

#include <memory>
#include <vector>

#include <QMainWindow>

class QEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The LaunchWindow class models the UI for a launchpad for opening new task templates and recent tasks.
 */
class LaunchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LaunchWindow();
    ~LaunchWindow();

    /**
     * @brief getTopLevelLaunchWindows Gets all of the existing top level launch windows.
     * @return The top level launch windows. Should only be empty if there are no launch windows open.
     */
    static std::vector<LaunchWindow*> getTopLevelLaunchWindows();

    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_actionGlobal_Preferences_triggered();
    void on_actionLoad_Global_Preferences_triggered();
    void on_actionSave_Global_Preferences_triggered();
    void on_actionClear_Recents_triggered();
    void on_actionExit_triggered();
    void on_actionTutorials_triggered();
    void on_actionSupport_triggered();
    void on_actionAbout_triggered();
    void on_openImageButton_clicked();
    void on_openLinkButton_clicked();
    void on_runScriptButton_clicked();

private:
    void closeEvent(QCloseEvent* bar) override;

    class LaunchWindowImpl;
    std::unique_ptr<LaunchWindowImpl> d;
};

}

}
