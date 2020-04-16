#include "imagetaskscriptingwidget.h"
#include "ui_imagetaskscriptingwidget.h"

#include <cassert>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

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

namespace
{

const std::string stopConditionIdPrefix = "stop_condition_";
const std::string beforeAddShapeCallbackPrefix = "before_add_shape_callback_";
const std::string afterAddShapeCallbackPrefix = "after_add_shape_callback_";

}

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
            const std::string defaultCode = "currentShapeCount >= 500; // Stop when there are more than 500 shapes";
            addScript(tr("Custom Stop Condition").toStdString(), ::stopConditionIdPrefix, defaultCode);
        });
        connect(ui->addBeforeAddShapeEvent, &QPushButton::clicked, [this]() {
            const std::string defaultCode = "printToAllScriptConsoleWidgets(\"Will add shape to image...\");";
            addScript(tr("Before Add Shape Callback").toStdString(), ::beforeAddShapeCallbackPrefix, defaultCode);
        });
        connect(ui->addAfterAddShapeEvent, &QPushButton::clicked, [this]() {
            const std::string defaultCode = "printToAllScriptConsoleWidgets(\"Did add shape to image...\");";
            addScript(tr("After Add Shape Callback").toStdString(), ::afterAddShapeCallbackPrefix, defaultCode);
        });
        connect(ui->clearScriptsButton, &QPushButton::clicked, [this]() {
            QLayoutItem* item = nullptr;
            while((item = ui->customScriptsEditorLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        });

        // Setup the actual actions that manipulate the script functions
        connect(q, &geometrize::dialog::ImageTaskScriptingWidget::signal_scriptChanged, [this](const std::string& functionName, const std::string& code) {
            if(m_task) {
                m_task->getPreferences().setScript(functionName, code);
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
        setScripts(m_task->getPreferences().getScripts());
        if(auto* shapeScriptingPanel = getShapeScriptingPanel()) {
            shapeScriptingPanel->syncUserInterface();
        }
    }

    std::map<std::string, std::string> getScripts() const
    {
        const auto& editors = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        std::map<std::string, std::string> m;
        for(ScriptEditorWidget* editor : editors) {
            m[editor->getFunctionName()] = editor->getCurrentCode();
        }
        return m;
    }

    void addScript(const std::string& scriptDisplayName, const std::string& scriptIdPrefix, const std::string& scriptCode)
    {
        const std::string functionName = scriptIdPrefix + QUuid::createUuid().toString().toStdString();
        auto widget = new geometrize::dialog::ScriptEditorWidget(scriptDisplayName, functionName, scriptCode, ui->customScriptsGroupBox);
        connect(widget, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
            emit q->signal_scriptChanged(functionName, code);
        });
        ui->customScriptsEditorLayout->addWidget(widget);
    }

    bool evaluateStopConditionScripts() const
    {
        const auto engine = m_task->getGeometrizer().getEngine();
        if(!engine) {
            return false;
        }

        const auto scriptWidgets = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        bool scriptStopConditionMet = false;
        for(const auto& widget : scriptWidgets) {
            if(widget->getFunctionName().substr(0, stopConditionIdPrefix.length()) != stopConditionIdPrefix) {
                continue; // Skip anything other than stop conditions
            }

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

    void evaluateBeforeAddShapeScripts()
    {
        const auto engine = m_task->getGeometrizer().getEngine();
        if(!engine) {
            return;
        }

        const auto scriptWidgets = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        for(const auto& widget : scriptWidgets) {
            if(widget->getFunctionName().substr(0, beforeAddShapeCallbackPrefix.length()) != beforeAddShapeCallbackPrefix) {
                continue;
            }

            try {
                engine->eval(widget->getCurrentCode());
                widget->onScriptEvaluationSucceeded();
            } catch(const chaiscript::exception::eval_error& e) {
                widget->onScriptEvaluationFailed(e.pretty_print());
            } catch(...) {
                widget->onScriptEvaluationFailed("Unknown script evaluation error");
            }
        }
    }

    void evaluateAfterAddShapeScripts()
    {
        const auto engine = m_task->getGeometrizer().getEngine();
        if(!engine) {
            return;
        }

        const auto scriptWidgets = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        for(const auto& widget : scriptWidgets) {
            if(widget->getFunctionName().substr(0, afterAddShapeCallbackPrefix.length()) != afterAddShapeCallbackPrefix) {
                continue;
            }

            try {
                engine->eval(widget->getCurrentCode());
                widget->onScriptEvaluationSucceeded();
            } catch(const chaiscript::exception::eval_error& e) {
                widget->onScriptEvaluationFailed(e.pretty_print());
            } catch(...) {
                widget->onScriptEvaluationFailed("Unknown script evaluation error");
            }
        }
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    // Utility function used to setup and display the shape creation/mutation script editor for the given image task window
    void revealShapeScriptingPanel()
    {
        if(dialog::ImageTaskShapeScriptingPanel* scriptingPanel = getShapeScriptingPanel()) {
            scriptingPanel->setWindowState(scriptingPanel->windowState() & ~Qt::WindowMinimized);
            QApplication::setActiveWindow(scriptingPanel);
            scriptingPanel->raise();
            scriptingPanel->show();
        }
    }

    void setScripts(const std::map<std::string, std::string>& scripts)
    {
        // Look for matching script editor widgets, and set their contents if found
        for(const auto& script : scripts) {
            if(dialog::ScriptEditorWidget* editor = findEditor(script.first)) {
                if(!script.second.empty()) {
                    editor->setCurrentCode(script.second);
                }
            } else {
                // TODO add the script if it starts with one of the predefined prefixes?
            }
        }
    }

    dialog::ScriptEditorWidget* findEditor(const std::string& editorId)
    {
        const auto& editors = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        for(const auto& editor : editors) {
            if(editor->getFunctionName() == editorId) {
                return editor;
            }
        }
        return nullptr;
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

bool ImageTaskScriptingWidget::evaluateStopConditionScripts() const
{
    return d->evaluateStopConditionScripts();
}

void ImageTaskScriptingWidget::evaluateBeforeAddShapeScripts() const
{
    d->evaluateBeforeAddShapeScripts();
}

void ImageTaskScriptingWidget::evaluateAfterAddShapeScripts() const
{
    d->evaluateAfterAddShapeScripts();
}

}

}
