#include "imagetaskscriptingwidget.h"
#include "ui_imagetaskscriptingwidget.h"

#include <cassert>
#include <cstdint>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <QComboBox>
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
#include "script/scriptutil.h"
#include "task/imagetask.h"

namespace
{

const std::string beforeStepCallbackPrefix = "before_step_callback_";
const std::string afterStepCallbackPrefix = "after_step_callback_";
const std::string stopConditionIdPrefix = "stop_condition_";
const std::string beforeAddShapeCallbackPrefix = "before_add_shape_callback_";
const std::string afterAddShapeCallbackPrefix = "after_add_shape_callback_";
const std::string onPenInputCallbackPrefix = "on_pen_input_callback_";

}

namespace geometrize
{

namespace dialog
{

void showImageTaskStopConditionMetMessage(QWidget* parent)
{
    QMessageBox info(QMessageBox::Icon::Information,
                     QObject::tr("Stop Condition Met"),
                     QObject::tr("Stop condition for geometrizing was met"),
                     QMessageBox::StandardButton::Ok,
                     parent);

    info.setWindowModality(Qt::WindowModality::WindowModal);
    info.setWindowFlags(info.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    info.exec();
}

class ImageTaskScriptingWidget::ImageTaskScriptingWidgetImpl
{
public:
    ImageTaskScriptingWidgetImpl(ImageTaskScriptingWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageTaskScriptingWidget>()}
    {
        ui->setupUi(q);
        populateUi();

        new geometrize::dialog::ImageTaskShapeScriptingPanel(q);

        // Populate the preset script comboboxes with the default/built-in scripts
        buildScriptSelectionComboBoxes();

        connect(ui->clearScriptsButton, &QPushButton::clicked, [this]() {
            QLayoutItem* item = nullptr;
            while((item = ui->customScriptsEditorLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        });
        connect(ui->rescanScriptsButton, &QPushButton::clicked, [this]() {
            buildScriptSelectionComboBoxes();
        });

        connect(ui->editShapeScriptsButton, &QPushButton::clicked, [this]() {
            revealShapeScriptingPanel();
        });

        connect(ui->addBeforeStepEvent, &QPushButton::clicked, [this]() {
            addBeforeStepCallbackWidget(getScriptForSelectedComboBoxItem(ui->addBeforeStepPresetScriptsComboBox));
        });
        connect(ui->addAfterStepEvent, &QPushButton::clicked, [this]() {
            addAfterStepCallbackWidget(getScriptForSelectedComboBoxItem(ui->addAfterStepPresetScriptsComboBox));
        });
        connect(ui->addStopConditionButton, &QPushButton::clicked, [this]() {
            addCustomStopConditionWidget(getScriptForSelectedComboBoxItem(ui->addStopConditionPresetScriptsComboBox));
        });
        connect(ui->addBeforeAddShapeEvent, &QPushButton::clicked, [this]() {
            addBeforeAddShapeCallbackWidget(getScriptForSelectedComboBoxItem(ui->addBeforeAddShapePresetScriptsComboBox));
        });
        connect(ui->addAfterAddShapeEvent, &QPushButton::clicked, [this]() {
            addAfterAddShapeCallbackWidget(getScriptForSelectedComboBoxItem(ui->addAfterAddShapePresetScriptsComboBox));
        });
        connect(ui->onScenePenInputEvent, &QPushButton::clicked, [this]() {
            addOnPenInputWidget(getScriptForSelectedComboBoxItem(ui->onScenePenInputEventPresetScriptsComboBox));
        });

        // Update the script code when the editor modifies them
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
        if(m_task == nullptr) {
            return;
        }

        setScripts(m_task->getPreferences().getScripts());
        getShapeScriptingPanel()->syncUserInterface();
    }

    std::map<std::string, std::string> getShapeMutationScripts()
    {
        return getShapeScriptingPanel()->getScripts();
    }

    // Note that we don't call this and apply the scripts to the image task until it's safe to do so (i.e. when the task isn't stepping)
    // As a result, the latest script settings won't be saved until after the image task has stepped (i.e. had the scripts applied to it)
    std::map<std::string, std::string> getScripts() const
    {
        const auto& editors = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        std::map<std::string, std::string> m;
        for(ScriptEditorWidget* editor : editors) {
            m[editor->getFunctionName()] = editor->getCurrentCode();
        }

        getShapeScriptingPanel()->syncUserInterface();

        getShapeScriptingPanel()->getScripts();

        return m;
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
            } catch(const std::exception& e) {
                widget->onScriptEvaluationFailed(e.what());
            } catch(...) {
                widget->onScriptEvaluationFailed("Unknown script evaluation error");
            }
        }

        return scriptStopConditionMet;
    }

    void evaluateScriptsWithNoReturnValue(const std::string& scriptGroupNamePrefix) const
    {
        const auto engine = m_task->getGeometrizer().getEngine();
        if(!engine) {
            return;
        }

        const auto scriptWidgets = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        for(const auto& widget : scriptWidgets) {
            if(widget->getFunctionName().substr(0, scriptGroupNamePrefix.length()) != scriptGroupNamePrefix) {
                continue;
            }

            try {
                engine->eval(widget->getCurrentCode());
                widget->onScriptEvaluationSucceeded();
            } catch(const chaiscript::exception::eval_error& e) {
                widget->onScriptEvaluationFailed(e.pretty_print());
            } catch(const std::exception& e) {
                widget->onScriptEvaluationFailed(e.what());
            } catch(...) {
                widget->onScriptEvaluationFailed("Unknown script evaluation error");
            }
        }
    }

    void evaluateBeforeStepScripts() const
    {
        evaluateScriptsWithNoReturnValue(::beforeStepCallbackPrefix);
    }

    void evaluateAfterStepScripts() const
    {
        evaluateScriptsWithNoReturnValue(::afterStepCallbackPrefix);
    }

    void evaluateBeforeAddShapeScripts() const
    {
        evaluateScriptsWithNoReturnValue(::beforeAddShapeCallbackPrefix);
    }

    void evaluateAfterAddShapeScripts() const
    {
        evaluateScriptsWithNoReturnValue(::afterAddShapeCallbackPrefix);
    }

    void evaluateOnPenInputEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onPenInputCallbackPrefix);
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    static bool startsWith(const std::string& s, const std::string& prefix)
    {
        return s.rfind(prefix, 0) == 0;
    }

    void addScriptWidget(const std::string& scriptDisplayName, const std::string& scriptIdPrefix, const std::string& scriptCode)
    {
        if(!startsWith(scriptIdPrefix, ::stopConditionIdPrefix) &&
           !startsWith(scriptIdPrefix, ::beforeAddShapeCallbackPrefix) &&
           !startsWith(scriptIdPrefix, ::afterAddShapeCallbackPrefix) &&
           !startsWith(scriptIdPrefix, ::beforeStepCallbackPrefix) &&
           !startsWith(scriptIdPrefix, ::afterStepCallbackPrefix) &&
           !startsWith(scriptIdPrefix, ::onPenInputCallbackPrefix))
        {
            return;
        }

        const std::string functionName = scriptIdPrefix + QUuid::createUuid().toString().toStdString();
        auto widget = new geometrize::dialog::ScriptEditorWidget(scriptDisplayName, functionName, scriptCode, ui->customScriptsGroupBox);
        connect(widget, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
            emit q->signal_scriptChanged(functionName, code);
        });
        ui->customScriptsEditorLayout->addWidget(widget);
    }

    void addBeforeStepCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("Before Step Callback").toStdString(), ::beforeStepCallbackPrefix, scriptCode);
    }

    void addAfterStepCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("After Step Callback").toStdString(), ::afterStepCallbackPrefix, scriptCode);
    }

    void addCustomStopConditionWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("Custom Stop Condition").toStdString(), ::stopConditionIdPrefix, scriptCode);
    }

    void addBeforeAddShapeCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("Before Add Shape Callback").toStdString(), ::beforeAddShapeCallbackPrefix, scriptCode);
    }

    void addAfterAddShapeCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("After Add Shape Callback").toStdString(), ::afterAddShapeCallbackPrefix, scriptCode);
    }

    void addOnPenInputWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Pen Input Callback").toStdString(), ::onPenInputCallbackPrefix, scriptCode);
    }

    // Utility function used to setup and display the shape creation/mutation script editor for the given image task window
    void revealShapeScriptingPanel()
    {
        auto scriptingPanel = getShapeScriptingPanel();
        scriptingPanel->setWindowState(scriptingPanel->windowState() & ~Qt::WindowMinimized);
        QApplication::setActiveWindow(scriptingPanel);
        scriptingPanel->raise();
        scriptingPanel->show();
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
                // If no such editor exists, create a new panel (but only for scripts with names we recognize)
                addScriptWidget(script.first, script.first, script.second);
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

    geometrize::dialog::ImageTaskShapeScriptingPanel* getShapeScriptingPanel() const
    {
        return q->findChild<geometrize::dialog::ImageTaskShapeScriptingPanel*>();
    }

    void populateUi()
    {
    }

    void populateScriptSelectionComboBox(QComboBox* combo, const std::map<std::string, std::string>& scripts)
    {
        combo->clear();
        for(const auto& item : scripts) {
            combo->addItem(QString::fromStdString(item.first), QString::fromStdString(item.second));
        }
        combo->setCurrentIndex(0);
    }

    std::string getScriptForSelectedComboBoxItem(QComboBox* combo)
    {
        return combo->itemData(combo->currentIndex()).toString().toStdString();
    }

    void buildScriptSelectionComboBoxes()
    {
        std::vector<std::pair<QComboBox*, std::map<std::string, std::string>>> presetScriptData;
        presetScriptData.push_back(std::make_pair(ui->addBeforeStepPresetScriptsComboBox, geometrize::script::getBeforeStepCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->addAfterStepPresetScriptsComboBox, geometrize::script::getAfterStepCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->addStopConditionPresetScriptsComboBox, geometrize::script::getStopConditionScripts()));
        presetScriptData.push_back(std::make_pair(ui->addBeforeAddShapePresetScriptsComboBox, geometrize::script::getBeforeAddShapeCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->addAfterAddShapePresetScriptsComboBox, geometrize::script::getAfterAddShapeCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->onScenePenInputEventPresetScriptsComboBox, geometrize::script::getOnPenInputCallbackScripts()));
        for(const auto& item : presetScriptData) {
            populateScriptSelectionComboBox(item.first, item.second);
        }
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

void ImageTaskScriptingWidget::evaluateBeforeStepScripts() const
{
    d->evaluateBeforeStepScripts();
}

void ImageTaskScriptingWidget::evaluateAfterStepScripts() const
{
    d->evaluateAfterStepScripts();
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

void ImageTaskScriptingWidget::evaluateOnPenInputEventScripts() const
{
    d->evaluateOnPenInputEventScripts();
}

std::map<std::string, std::string> ImageTaskScriptingWidget::getShapeMutationScripts() const
{
    return d->getShapeMutationScripts();
}

}

}
