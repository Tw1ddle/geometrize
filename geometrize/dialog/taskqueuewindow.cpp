#include "taskqueuewindow.h"
#include "ui_taskqueuewindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUuid>

#include "chaiscript/chaiscript.hpp"

#include "common/formatsupport.h"
#include "dialog/scripteditorwidget.h"
#include "dialog/taskitemwidget.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "task/taskutil.h"

namespace
{

const std::string defaultScriptCode = R"(// Load up the image file
var image = loadImage(inputPath);
if(image.isNull()) {
    messageBox("Failed to load image from: " + inputPath);
    return;
}

// Make sure the image is in RGBA8888 format
image := convertImageToRgba8888(image);
if(image.isNull()) {
    messageBox("Failed to convert image to the proper format");
    return;
}

// Create a RGBA8888 bitmap from the image
// Note that this downscales the image as well (to reduce processing time)
var bitmap = convertImageToBitmapWithDownscaling(image);

// Create an image task
var task = createImageTask(bitmap);

// Create some image task preferences
var prefs = ImageTaskPreferences();

prefs.setCandidateShapeCount(200);
prefs.setMaxShapeMutations(200);
prefs.setShapeAlpha(255);
prefs.setSeed(1000);
prefs.setShapeTypes(RECTANGLE);
prefs.enableShapeTypes(TRIANGLE);
prefs.enableShapeTypes(ROTATED_RECTANGLE);

// Only use one thread per task
prefs.setMaxThreads(1);

// Define a stop condition script that will stop the geometrization after a while
def getStopConditionCode() {
  return "return currentShapeCount >= 100;";
}

// Define code to run when the stop condition is met
// Saves the files in a folder on the user's desktop
def getOnStopConditionMetCode() {
  var outputDir = getDesktopDirectoryLocation() + "/geometrize_batch_output/";
  createDirectory(outputDir);

  var outputPath = outputDir + "/result_" + getFilenameTimestamp() +"_" + getUuidString() + ".png";
  return "exportBitmap(task.getCurrent()," + "\"" + outputPath + "\"" + ");";
}

// Set a stop condition that will stop the task when the condition is met
prefs.setScript("stop_condition_for_script", getStopConditionCode());

// Save the image when the stop condition is met
prefs.setScript("on_stop_condition_met_for_script", getOnStopConditionMetCode());

// Set the preferences up on the image task
task.setPreferences(prefs);

// Create the image task UI window
var imageTaskWindow = createImageTaskWindow();

// Set the image task up on the UI
imageTaskWindow.setImageTask(task);

// Show the image task window
imageTaskWindow.show();

// Give the window an id we can use to refer to it later
var name = getUuidString();
imageTaskWindow.setCommandHandlerName(name);

// Hide a bunch of unnecessary things that will slow down the batch processing
sendCommandString(name, "hide_pixmap_view");
sendCommandString(name, "hide_vector_view");
sendCommandString(name, "hide_script_console");

// Tell the window to click the start button
sendCommandString(name, "start");

)";

}

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

        setupScriptEditor();

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
        QListWidgetItem* item{new QListWidgetItem()};
        dialog::TaskItemWidget* button{new dialog::TaskItemWidget(itemPath, itemDisplayName,
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
        const std::string functionName = "task_queue_processing_script" + QUuid::createUuid().toString().toStdString();
        const std::string scriptEditorWidgetTitle = tr("Script Editor").toStdString();

        m_scriptEditorWidget = new geometrize::dialog::ScriptEditorWidget(scriptEditorWidgetTitle, functionName, defaultScriptCode, ui->taskListScriptContainer);

        connect(m_scriptEditorWidget, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
            emit q->signal_scriptChanged(functionName, code);
        });

        ui->taskListScriptLayout->addWidget(m_scriptEditorWidget);
    }

    void populateUi()
    {
    }

    std::unique_ptr<Ui::TaskQueueWindow> ui{nullptr};
    TaskQueueWindow* q{nullptr};

    geometrize::dialog::ScriptEditorWidget* m_scriptEditorWidget;
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
        const QString urlString{url.toLocalFile()};
        tasks.push_back(urlString);
    }
    d->addItems(tasks);
}

}

}
