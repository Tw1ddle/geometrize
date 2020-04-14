#include "taskqueuewindow.h"
#include "ui_taskqueuewindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>

#include "common/formatsupport.h"
#include "dialog/taskitemwidget.h"
#include "task/taskutil.h"

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

        connect(ui->startStopButton, &QPushButton::clicked, []() {

        });
        connect(ui->clearQueueButton, &QPushButton::clicked, [this]() {
            ui->taskList->clear();
        });
    }
    TaskQueueWindowImpl& operator=(const TaskQueueWindowImpl&) = delete;
    TaskQueueWindowImpl(const TaskQueueWindowImpl&) = delete;
    ~TaskQueueWindowImpl()
    {
    }

    void addItems(const QStringList& tasks)
    {
        for(const auto& task : tasks) {
            addItem(task, task);
        }
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
    void addItem(const QString& itemPath, const QString& itemDisplayName) const
    {
        QListWidgetItem* item{new QListWidgetItem()};
        dialog::TaskItemWidget* button{new dialog::TaskItemWidget(itemPath, itemDisplayName,
        [item](const QString& taskItemId) {
            geometrize::util::openTasks({taskItemId}, false);
        },
        [item](const QString& /*taskItemId*/) {
            delete item;
        })};
        item->setToolTip(itemPath);
        item->setSizeHint(button->sizeHint());
        setMenuItemKey(item, itemPath);
        ui->taskList->addItem(item);
        ui->taskList->setItemWidget(item, button);
    }

    void setMenuItemKey(QListWidgetItem* item, const QString& key) const
    {
        item->setData(Qt::UserRole, key);
    }

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

void TaskQueueWindow::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void TaskQueueWindow::dropEvent(QDropEvent* event)
{
    const QList<QUrl> urls{geometrize::format::getUrls(event->mimeData())};
    QStringList tasks;
    for(const QUrl& url : urls) {
        const QString urlString{url.toString()};
        tasks.push_back(urlString);
    }
    d->addItems(tasks);
}

}

}
