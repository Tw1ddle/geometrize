#pragma once

#include <memory>

#include <QMainWindow>

class QEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The TaskQueueWindow class encapsulates the UI for creating, configuring and executing a collection of tasks that geometrize images.
 */
class TaskQueueWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TaskQueueWindow();
    virtual ~TaskQueueWindow();

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

    void changeEvent(QEvent*) override;

private slots:
    void on_actionExit_triggered();

signals:
    void signal_scriptChanged(const std::string& functionName, const std::string& code);

private:
    class TaskQueueWindowImpl;
    std::unique_ptr<TaskQueueWindowImpl> d;
};

}

}
