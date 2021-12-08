#include "taskqueuewindow.h"
#include "ui_taskqueuewindow.h"

#include <QCollator>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QTimer>
#include <QUuid>

#include "chaiscript/chaiscript.hpp"

#include "common/formatsupport.h"
#include "dialog/imagetaskwindow.h"
#include "dialog/recenttaskslist.h"
#include "dialog/scripteditorwidget.h"
#include "dialog/taskitemwidget.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "script/scriptutil.h"
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

        refreshScripts();
        setupScriptEditor();

        connect(ui->scriptSelectComboBox, &QComboBox::currentTextChanged, [this](const QString& text) {
            m_scriptEditorWidget->setCurrentCode(m_scripts[text.toStdString()]);
        });

        connect(ui->runTasksButton, &QPushButton::clicked, [this]() {
            for(int i = 0; i < ui->taskList->count(); ++i) {
                QListWidgetItem* item = ui->taskList->item(i);
                QString data = item->data(Qt::UserRole).toString();
                runScript(data.toStdString());
            }
        });
        connect(ui->clearTaskListButton, &QPushButton::clicked, [this]() {
            ui->taskList->clear();
        });
        connect(ui->closeOpenWindowsButton, &QPushButton::clicked, [this]() {
            closeOpenWindows();
        });

        // Enable the run and clear buttons only when items are present
        ui->runTasksButton->setEnabled(false);
        ui->clearTaskListButton->setEnabled(false);
        QTimer* timer = new QTimer(q);
        q->connect(timer, &QTimer::timeout, q, [this]() {
            const bool enableRunClearButtons = ui->taskList->count() > 0;
            ui->runTasksButton->setEnabled(enableRunClearButtons);
            ui->clearTaskListButton->setEnabled(enableRunClearButtons);
            ui->closeOpenWindowsButton->setEnabled(!geometrize::dialog::ImageTaskWindow::getExistingImageTaskWindows().empty());
        });
        timer->start(500);
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

    void closeOpenWindows()
    {
        const auto& windows = geometrize::dialog::ImageTaskWindow::getExistingImageTaskWindows();
        for(auto* window : windows) {
            window->close();
        }
    }

private:
    void refreshScripts()
    {
        m_scripts = geometrize::script::getTaskQueueBatchProcessingScripts();

        ui->scriptSelectComboBox->clear();
        for(const auto& script : m_scripts) {
            ui->scriptSelectComboBox->addItem(QString::fromStdString(script.first));
        }
    }

    void runScript(const std::string& imagePath) const
    {
        const auto engine = script::createBatchImageTaskEngine();
        engine->set_global(chaiscript::var(imagePath), "inputPath");
        const auto& code = m_scriptEditorWidget->getCurrentCode();
        try {
            engine->eval(code);
            m_scriptEditorWidget->onScriptEvaluationSucceeded();
        } catch(const chaiscript::exception::eval_error& e) {
            m_scriptEditorWidget->onScriptEvaluationFailed(e.pretty_print());
        } catch(...) {
            m_scriptEditorWidget->onScriptEvaluationFailed("Unknown script evaluation error");
        }
    }

    void addItem(const QString& itemPath, const QString& itemDisplayName) const
    {
        class MyListWidgetItem : public QListWidgetItem {
        public:
            MyListWidgetItem()
            {
                static int instanceCounter = 0;
                m_instanceId = instanceCounter;
                instanceCounter++;
                //setData(Qt::DisplayRole, instanceCounter);
            }

            virtual bool operator<(const MyListWidgetItem& other) const {
                return m_instanceId > other.m_instanceId;
            }

        private:
            int m_instanceId;
        };

        QListWidgetItem* item{new MyListWidgetItem()};
        dialog::TaskItemWidget* button{new dialog::TaskItemWidget(itemPath, RecentTasksList::getDisplayNameForTaskPath(itemDisplayName),
        [this](const QString& taskItemId) {
            runScript(taskItemId.toStdString());
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

    void setupScriptEditor()
    {
        const std::string scriptEditorWidgetTitle = tr("Script Editor").toStdString();

        m_scriptEditorWidget = new geometrize::dialog::ScriptEditorWidget(scriptEditorWidgetTitle, defaultScriptName, m_scripts.at(defaultScriptName), ui->taskListScriptContainer);

        connect(m_scriptEditorWidget, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
            emit q->signal_scriptChanged(functionName, code);
        });

        ui->scrollArea->setWidget(m_scriptEditorWidget);
    }

    void populateUi()
    {
    }

    std::unique_ptr<Ui::TaskQueueWindow> ui{nullptr};
    TaskQueueWindow* q{nullptr};

    geometrize::dialog::ScriptEditorWidget* m_scriptEditorWidget;

    const std::string defaultScriptName{ "default" };
    std::map<std::string, std::string> m_scripts;
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
