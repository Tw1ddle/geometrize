#include "imagetaskscriptingwidget.h"
#include "ui_imagetaskscriptingwidget.h"

#include <cassert>
#include <cstdint>
#include <memory>

#include <QEvent>
#include <QList>
#include <QLocale>
#include <QMessageBox>
#include <QString>
#include <QUuid>

#include "chaiscript/chaiscript.hpp"

#include "dialog/imagetaskshapescriptingpanel.h"
#include "dialog/scripteditorwidget.h"
#include "preferences/globalpreferences.h"
#include "script/geometrizerengine.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace dialog
{

void showImageTaskStopConditionMetMessage(QWidget* parent)
{
    QMessageBox::information(parent, QObject::tr("Stop Condition Met"), QObject::tr("Stop condition for geometrizing was met"));
}

class ImageTaskScriptingWidget::ImageTaskScriptingWidgetImpl
{
public:
    ImageTaskScriptingWidgetImpl(ImageTaskScriptingWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageTaskScriptingWidget>()}
    {
        ui->setupUi(q);
        populateUi();

        new geometrize::dialog::ImageTaskShapeScriptingPanel(q);

        connect(ui->editShapeScriptsButton, &QPushButton::clicked, [this]() {
            revealShapeScriptingPanel();
        });
        connect(ui->addStopConditionButton, &QPushButton::clicked, [this]() {
            const std::string defaultCode = "shapeCount >= 500;";
            addStopCondition(defaultCode);
        });
        connect(ui->clearScriptsButton, &QPushButton::clicked, [this]() {
            QLayoutItem* item = nullptr;
            while((item = ui->scriptsEditorLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        });

        // Reveal shape scripting panel if global preferences are set this way
        const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
        if(prefs.shouldShowImageTaskScriptEditorByDefault()) {
            revealShapeScriptingPanel();
        }
    }
    ~ImageTaskScriptingWidgetImpl() = default;
    ImageTaskScriptingWidgetImpl operator=(const ImageTaskScriptingWidgetImpl&) = delete;
    ImageTaskScriptingWidgetImpl(const ImageTaskScriptingWidgetImpl&) = delete;

    void setImageTask(task::ImageTask* task)
    {
        m_task = task;
        getShapeScriptingPanel()->setImageTask(task);
    }

    void syncUserInterface()
    {

    }

    void addStopCondition(const std::string& scriptCode)
    {
        const std::string editorName = tr("Custom Stop Condition").toStdString();
        const std::string functionName = "stop_condition_" + QUuid::createUuid().toString().toStdString();
        auto widget = new geometrize::dialog::ScriptEditorWidget(editorName, "", scriptCode, ui->customScriptsGroupBox);
        ui->scriptsEditorLayout->addWidget(widget);
    }

    bool evaluateStopConditions(const std::size_t currentShapeCount) const
    {
        const auto engine = m_task->getGeometrizer().getEngine();
        if(!engine) {
            return false;
        }

        engine->set_global(chaiscript::var(currentShapeCount), "shapeCount");

        const auto scriptWidgets = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        if(scriptWidgets.empty()) {
            return false;
        }

        bool scriptStopConditionMet = false;
        for(const auto& widget : scriptWidgets) {
            try {
                auto stopCondition = engine->eval<bool>(widget->getCurrentCode());
                if(stopCondition) {
                    scriptStopConditionMet = true;
                }
                widget->onScriptEvaluationSucceeded();
            } catch(const chaiscript::exception::eval_error& e) {
                widget->onScriptEvaluationFailed(e.pretty_print());
            } catch(...) {
                widget->onScriptEvaluationFailed("Unknown script evaluation error");
            }
        }

        return scriptStopConditionMet;
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    // Utility function used to setup and display the script editor for the given image task window
    void revealShapeScriptingPanel()
    {
        if(dialog::ImageTaskShapeScriptingPanel* scriptingPanel = getShapeScriptingPanel()) {
            scriptingPanel->setWindowState(scriptingPanel->windowState() & ~Qt::WindowMinimized);
            QApplication::setActiveWindow(scriptingPanel);
            scriptingPanel->raise();
            scriptingPanel->show();
        }
    }

    geometrize::dialog::ImageTaskShapeScriptingPanel* getShapeScriptingPanel()
    {
        return q->findChild<geometrize::dialog::ImageTaskShapeScriptingPanel*>();
    }

    void populateUi()
    {
    }

    task::ImageTask* m_task{nullptr};

    ImageTaskScriptingWidget* q;
    std::unique_ptr<Ui::ImageTaskScriptingWidget> ui;
};

ImageTaskScriptingWidget::ImageTaskScriptingWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskScriptingWidget::ImageTaskScriptingWidgetImpl>(this)}
{
}

ImageTaskScriptingWidget::~ImageTaskScriptingWidget()
{
}

void ImageTaskScriptingWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

void ImageTaskScriptingWidget::setImageTask(task::ImageTask* task)
{
    d->setImageTask(task);
}

void ImageTaskScriptingWidget::syncUserInterface()
{
    d->syncUserInterface();
}

bool ImageTaskScriptingWidget::evaluateStopConditions(const std::size_t currentShapeCount) const
{
    return d->evaluateStopConditions(currentShapeCount);
}

}

}
