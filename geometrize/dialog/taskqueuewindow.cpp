#include "taskqueuewindow.h"
#include "ui_taskqueuewindow.h"

namespace geometrize
{

namespace dialog
{

class TaskQueueWindow::TaskQueueWindowImpl
{
public:
    TaskQueueWindowImpl(TaskQueueWindow* pQ) :
        ui{std::make_unique<Ui::TaskQueueWindow>()},
        q{pQ}
    {
        ui->setupUi(q);
        populateUi();
        q->setAttribute(Qt::WA_DeleteOnClose);
    }
    TaskQueueWindowImpl& operator=(const TaskQueueWindowImpl&) = delete;
    TaskQueueWindowImpl(const TaskQueueWindowImpl&) = delete;
    ~TaskQueueWindowImpl()
    {
    }

    void close()
    {
        q->close();
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {
    }

    std::unique_ptr<Ui::TaskQueueWindow> ui{nullptr};
    TaskQueueWindow* q{nullptr};
};

TaskQueueWindow::TaskQueueWindow() :
    QMainWindow{nullptr},
    d{std::make_unique<TaskQueueWindow::TaskQueueWindowImpl>(this)}
{
}

TaskQueueWindow::~TaskQueueWindow()
{
}

void TaskQueueWindow::on_actionExit_triggered()
{
    d->close();
}

void TaskQueueWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QMainWindow::changeEvent(event);
}

}

}
