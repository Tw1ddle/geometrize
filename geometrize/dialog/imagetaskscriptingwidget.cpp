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
const std::string addShapePreconditionCallbackPrefix = "add_shape_precondition_";
const std::string onStopConditionMetCallbackPrefix = "on_stop_condition_met_";
const std::string beforeAddShapeCallbackPrefix = "before_add_shape_callback_";
const std::string afterAddShapeCallbackPrefix = "after_add_shape_callback_";
const std::string onPenInputCallbackPrefix = "on_pen_input_callback_";
const std::string onPenProximityEnterCallbackPrefix = "on_pen_proximity_enter_callback_";
const std::string onPenProximityExitCallbackPrefix = "on_pen_proximity_exit_callback_";
const std::string onKeyDownCallbackPrefix = "on_key_down_callback_";
const std::string onKeyUpCallbackPrefix = "on_key_up_callback_";
const std::string onMouseDownCallbackPrefix = "on_mouse_down_callback_";
const std::string onMouseUpCallbackPrefix = "on_mouse_up_callback_";
const std::string onMouseMoveCallbackPrefix = "on_mouse_move_callback_";
const std::string onMouseWheelCallbackPrefix = "on_mouse_wheel_callback_";
const std::string onTimedUpdateCallbackPrefix = "on_timed_update_callback_";

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
        connect(ui->addShapePreconditionButton, &QPushButton::clicked, [this]() {
            addShapePreconditionWidget(getScriptForSelectedComboBoxItem(ui->addShapePreconditionPresetScriptsComboBox));
        });
        connect(ui->onStopConditionMetEvent, &QPushButton::clicked, [this]() {
            addOnStopConditionMetWidget(getScriptForSelectedComboBoxItem(ui->onStopConditionMetEventComboBox));
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
        connect(ui->onPenProximityEnterEvent, &QPushButton::clicked, [this]() {
            addOnPenProximityEnterWidget(getScriptForSelectedComboBoxItem(ui->onPenProximityEnterEventPresetScriptsComboBox));
        });
        connect(ui->onPenProximityExitEvent, &QPushButton::clicked, [this]() {
            addOnPenProximityExitWidget(getScriptForSelectedComboBoxItem(ui->onPenProximityExitEventPresetScriptsComboBox));
        });
        connect(ui->onKeyDownEvent, &QPushButton::clicked, [this]() {
            addOnKeyDownWidget(getScriptForSelectedComboBoxItem(ui->onKeyDownEventPresetScriptsComboBox));
        });
        connect(ui->onKeyUpEvent, &QPushButton::clicked, [this]() {
            addOnKeyUpWidget(getScriptForSelectedComboBoxItem(ui->onKeyUpEventPresetScriptsComboBox));
        });
        connect(ui->onMouseDownEvent, &QPushButton::clicked, [this]() {
            addOnMouseDownWidget(getScriptForSelectedComboBoxItem(ui->onMouseDownEventPresetScriptsComboBox));
        });
        connect(ui->onMouseUpEvent, &QPushButton::clicked, [this]() {
            addOnMouseUpWidget(getScriptForSelectedComboBoxItem(ui->onMouseUpEventPresetScriptsComboBox));
        });
        connect(ui->onMouseMoveEvent, &QPushButton::clicked, [this]() {
            addOnMouseMoveWidget(getScriptForSelectedComboBoxItem(ui->onMouseMoveEventPresetScriptsComboBox));
        });
        connect(ui->onMouseWheelEvent, &QPushButton::clicked, [this]() {
            addOnMouseWheelWidget(getScriptForSelectedComboBoxItem(ui->onMouseWheelEventPresetScriptsComboBox));
        });
        connect(ui->onUpdateEvent, &QPushButton::clicked, [this]() {
            addOnTimedUpdateWidget(getScriptForSelectedComboBoxItem(ui->onUpdateEventPresetScriptsComboBox));
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
        // Gather the scripts from the editor boxes
        const auto& editors = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        std::map<std::string, std::string> scripts;
        for(ScriptEditorWidget* editor : editors) {
            scripts[editor->getFunctionName()] = editor->getCurrentCode();
        }

        // Gather the scripts from the shape scripting panel
        const std::map<std::string, std::string> shapeScripts = getShapeScriptingPanel()->getScripts();
        scripts.insert(shapeScripts.begin(), shapeScripts.end());

        return scripts;
    }

    bool evaluateScriptsWithBooleanReturnValue(const std::string& scriptPrefix) const
    {
        const auto engine = m_task->getGeometrizer().getEngine();
        if(!engine) {
            return false;
        }

        const auto scriptWidgets = ui->customScriptsGroupBox->findChildren<geometrize::dialog::ScriptEditorWidget*>();
        bool returnValue = false;
        for(const auto& widget : scriptWidgets) {
            if(widget->getFunctionName().substr(0, scriptPrefix.length()) != scriptPrefix) {
                continue; // Skip anything other than stop conditions
            }

            try {
                auto retValue = engine->eval<bool>(widget->getCurrentCode());
                if(retValue) {
                    returnValue = true;
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

        return returnValue;
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

    bool evaluateStopConditionScripts() const
    {
        return evaluateScriptsWithBooleanReturnValue(::stopConditionIdPrefix);
    }

    void evaluateOnStopConditionMetScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onStopConditionMetCallbackPrefix);
    }

    void evaluateBeforeStepScripts() const
    {
        evaluateScriptsWithNoReturnValue(::beforeStepCallbackPrefix);
    }

    void evaluateAfterStepScripts() const
    {
        evaluateScriptsWithNoReturnValue(::afterStepCallbackPrefix);
    }

    bool evaluateAddShapePreconditionScripts() const
    {
        return evaluateScriptsWithBooleanReturnValue(::addShapePreconditionCallbackPrefix);
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

    void evaluateOnPenProximityEnterEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onPenProximityEnterCallbackPrefix);
    }

    void evaluateOnPenProximityExitEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onPenProximityExitCallbackPrefix);
    }

    void evaluateOnKeyDownEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onKeyDownCallbackPrefix);
    }

    void evaluateOnKeyUpEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onKeyUpCallbackPrefix);
    }

    void evaluateOnMouseDownEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onMouseDownCallbackPrefix);
    }

    void evaluateOnMouseUpEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onMouseUpCallbackPrefix);
    }

    void evaluateOnMouseMoveEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onMouseMoveCallbackPrefix);
    }

    void evaluateOnMouseWheelEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onMouseWheelCallbackPrefix);
    }

    void evaluateOnTimedUpdateEventScripts() const
    {
        evaluateScriptsWithNoReturnValue(::onTimedUpdateCallbackPrefix);
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

    void addScriptWidget(const std::string& scriptDisplayName, const std::string& functionName, const std::string& scriptCode)
    {
        if(!startsWith(functionName, ::stopConditionIdPrefix) &&
           !startsWith(functionName, ::onStopConditionMetCallbackPrefix) &&
           !startsWith(functionName, ::addShapePreconditionCallbackPrefix) &&
           !startsWith(functionName, ::beforeAddShapeCallbackPrefix) &&
           !startsWith(functionName, ::afterAddShapeCallbackPrefix) &&
           !startsWith(functionName, ::beforeStepCallbackPrefix) &&
           !startsWith(functionName, ::afterStepCallbackPrefix) &&
           !startsWith(functionName, ::onPenInputCallbackPrefix) &&
           !startsWith(functionName, ::onPenProximityEnterCallbackPrefix) &&
           !startsWith(functionName, ::onPenProximityExitCallbackPrefix) &&
           !startsWith(functionName, ::onKeyDownCallbackPrefix) &&
           !startsWith(functionName, ::onKeyUpCallbackPrefix) &&
           !startsWith(functionName, ::onMouseDownCallbackPrefix) &&
           !startsWith(functionName, ::onMouseUpCallbackPrefix) &&
           !startsWith(functionName, ::onMouseMoveCallbackPrefix) &&
           !startsWith(functionName, ::onMouseWheelCallbackPrefix) &&
           !startsWith(functionName, ::onTimedUpdateCallbackPrefix))
        {
            return;
        }

        auto widget = new geometrize::dialog::ScriptEditorWidget(scriptDisplayName, functionName, scriptCode, ui->customScriptsGroupBox);
        connect(widget, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
            emit q->signal_scriptChanged(functionName, code);
        });
        ui->customScriptsEditorLayout->addWidget(widget);
    }

    void addBeforeStepCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("Before Step Callback").toStdString(), makeUniqueFunctionName(::beforeStepCallbackPrefix), scriptCode);
    }

    void addAfterStepCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("After Step Callback").toStdString(), makeUniqueFunctionName(::afterStepCallbackPrefix), scriptCode);
    }

    void addCustomStopConditionWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("Custom Stop Condition").toStdString(), makeUniqueFunctionName(::stopConditionIdPrefix), scriptCode);
    }

    void addOnStopConditionMetWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Stop Condition Met Callback").toStdString(), makeUniqueFunctionName(::onStopConditionMetCallbackPrefix), scriptCode);
    }

    void addShapePreconditionWidget(const std::string& scriptCode)
    {
         addScriptWidget(tr("Add Shape Precondition").toStdString(), makeUniqueFunctionName(::addShapePreconditionCallbackPrefix), scriptCode);
    }

    void addBeforeAddShapeCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("Before Add Shape Callback").toStdString(), makeUniqueFunctionName(::beforeAddShapeCallbackPrefix), scriptCode);
    }

    void addAfterAddShapeCallbackWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("After Add Shape Callback").toStdString(), makeUniqueFunctionName(::afterAddShapeCallbackPrefix), scriptCode);
    }

    void addOnPenInputWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Pen Input Callback").toStdString(), makeUniqueFunctionName(::onPenInputCallbackPrefix), scriptCode);
    }

    void addOnPenProximityEnterWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Pen Proximity Enter Callback").toStdString(), makeUniqueFunctionName(::onPenProximityEnterCallbackPrefix), scriptCode);
    }

    void addOnPenProximityExitWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Pen Proximity Exit Callback").toStdString(), makeUniqueFunctionName(::onPenProximityExitCallbackPrefix), scriptCode);
    }

    void addOnKeyDownWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Key Down Callback").toStdString(), makeUniqueFunctionName(::onKeyDownCallbackPrefix), scriptCode);
    }

    void addOnKeyUpWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Key Up Callback").toStdString(), makeUniqueFunctionName(::onKeyUpCallbackPrefix), scriptCode);
    }

    void addOnMouseDownWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Mouse Down Callback").toStdString(), makeUniqueFunctionName(::onMouseDownCallbackPrefix), scriptCode);
    }

    void addOnMouseUpWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Mouse Up Callback").toStdString(), makeUniqueFunctionName(::onMouseUpCallbackPrefix), scriptCode);
    }

    void addOnMouseMoveWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Mouse Move Callback").toStdString(), makeUniqueFunctionName(::onMouseMoveCallbackPrefix), scriptCode);
    }

    void addOnMouseWheelWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Mouse Wheel Callback").toStdString(), makeUniqueFunctionName(::onMouseWheelCallbackPrefix), scriptCode);
    }

    void addOnTimedUpdateWidget(const std::string& scriptCode)
    {
        addScriptWidget(tr("On Timed Update Callback").toStdString(), makeUniqueFunctionName(::onTimedUpdateCallbackPrefix), scriptCode);
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
        presetScriptData.push_back(std::make_pair(ui->onStopConditionMetEventComboBox, geometrize::script::getOnStopConditionMetScripts()));
        presetScriptData.push_back(std::make_pair(ui->addShapePreconditionPresetScriptsComboBox, geometrize::script::getAddShapePreconditionScripts()));
        presetScriptData.push_back(std::make_pair(ui->addBeforeAddShapePresetScriptsComboBox, geometrize::script::getBeforeAddShapeCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->addAfterAddShapePresetScriptsComboBox, geometrize::script::getAfterAddShapeCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->onScenePenInputEventPresetScriptsComboBox, geometrize::script::getOnPenInputCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->onPenProximityEnterEventPresetScriptsComboBox, geometrize::script::getOnPenProximityEnterCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->onPenProximityExitEventPresetScriptsComboBox, geometrize::script::getOnPenProximityExitCallbackScripts()));
        presetScriptData.push_back(std::make_pair(ui->onKeyDownEventPresetScriptsComboBox, geometrize::script::getOnKeyDownEventScripts()));
        presetScriptData.push_back(std::make_pair(ui->onKeyUpEventPresetScriptsComboBox, geometrize::script::getOnKeyUpEventScripts()));
        presetScriptData.push_back(std::make_pair(ui->onMouseDownEventPresetScriptsComboBox, geometrize::script::getOnMouseDownEventScripts()));
        presetScriptData.push_back(std::make_pair(ui->onMouseUpEventPresetScriptsComboBox, geometrize::script::getOnMouseUpEventScripts()));
        presetScriptData.push_back(std::make_pair(ui->onMouseMoveEventPresetScriptsComboBox, geometrize::script::getOnMouseMoveEventScripts()));
        presetScriptData.push_back(std::make_pair(ui->onMouseWheelEventPresetScriptsComboBox, geometrize::script::getOnMouseWheelEventScripts()));
        presetScriptData.push_back(std::make_pair(ui->onUpdateEventPresetScriptsComboBox, geometrize::script::getOnTimedUpdateEventScripts()));

        for(const auto& item : presetScriptData) {
            populateScriptSelectionComboBox(item.first, item.second);
        }
    }

    std::string makeUniqueFunctionName(const std::string& prefix)
    {
        return prefix + QUuid::createUuid().toString().toStdString();
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

void ImageTaskScriptingWidget::evaluateOnStopConditionMetScripts() const
{
    d->evaluateOnStopConditionMetScripts();
}

bool ImageTaskScriptingWidget::evaluateAddShapePreconditionScripts() const
{
    return d->evaluateAddShapePreconditionScripts();
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

void ImageTaskScriptingWidget::evaluateOnPenProximityEnterEventScripts() const
{
    d->evaluateOnPenProximityEnterEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnPenProximityExitEventScripts() const
{
    d->evaluateOnPenProximityExitEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnKeyDownEventScripts() const
{
    d->evaluateOnKeyDownEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnKeyUpEventScripts() const
{
    d->evaluateOnKeyUpEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnMouseDownEventScripts() const
{
    d->evaluateOnMouseDownEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnMouseUpEventScripts() const
{
    d->evaluateOnMouseUpEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnMouseMoveEventScripts() const
{
    d->evaluateOnMouseMoveEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnMouseWheelEventScripts() const
{
    d->evaluateOnMouseWheelEventScripts();
}

void ImageTaskScriptingWidget::evaluateOnTimedUpdateEventScripts() const
{
    d->evaluateOnTimedUpdateEventScripts();
}

std::map<std::string, std::string> ImageTaskScriptingWidget::getScripts() const
{
    return d->getScripts();
}

}

}
