#include "imagetaskshapescriptingpanel.h"
#include "ui_imagetaskshapescriptingpanel.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>

#include <QComboBox>
#include <QEvent>
#include <QPushButton>
#include <QStringList>

#include "dialog/imagetaskshapescriptingpanel.h"
#include "dialog/scripteditorwidget.h"
#include "script/geometrizerengine.h"
#include "script/scriptutil.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace dialog
{

class ImageTaskShapeScriptingPanel::ImageTaskShapeScriptingPanelImpl
{
public:
    ImageTaskShapeScriptingPanelImpl(ImageTaskShapeScriptingPanel* pQ) : ui{std::make_unique<Ui::ImageTaskShapeScriptingPanel>()}, q{pQ}
    {
        q->setWindowFlags(Qt::Window);
        ui->setupUi(q);

        // Setup the scripting enable/disable checkbox
        connect(ui->scriptsEnabledButton, &QCheckBox::toggled, [this](const bool enabled) {
            emit q->signal_scriptingToggled(enabled);
        });

        // Setup the reset scripting engine checkbox
        connect(ui->resetScriptEngineButton, &QPushButton::pressed, [this]() {
            for(ScriptEditorWidget* editor : m_editors) {
                editor->resetCodeToDefault();
            }
            emit q->signal_scriptsReset();
        });

        // Enable/disable scripting
        connect(q, &geometrize::dialog::ImageTaskShapeScriptingPanel::signal_scriptingToggled, [this](const bool enableScripting) {
            setScriptModeEnabled(enableScripting);
        });

        // Resets the scripts to their original presets when reset is pressed
        connect(ui->resetScriptEngineButton, &QPushButton::pressed, [this]() {
            for(ScriptEditorWidget* editor : m_editors) {
                editor->resetCodeToDefault();
            }
        });

        // Setup the actual actions that manipulate the image geometrization script functions
        connect(q, &geometrize::dialog::ImageTaskShapeScriptingPanel::signal_scriptChanged, [this](const std::string& functionName, const std::string& code) {
            if(m_task) {
                m_task->getPreferences().setScript(functionName, code);
            }
        });

        // Handle changes to the script presets
        connect(ui->scriptsPresetsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](const int idx) {
            if(idx >= 0) {
                setupPreset(idx);
            }
        });

        connect(ui->rescanScriptFoldersButton, &QPushButton::pressed, [this]() {
            buildPresetsMenuAndSelectPreset();
        });

        buildPresetsMenuAndSelectPreset();
    }
    ~ImageTaskShapeScriptingPanelImpl() = default;
    ImageTaskShapeScriptingPanelImpl operator=(const ImageTaskShapeScriptingPanelImpl&) = delete;
    ImageTaskShapeScriptingPanelImpl(const ImageTaskShapeScriptingPanelImpl&) = delete;

    void setImageTask(task::ImageTask* task)
    {
        m_task = task;

        if(m_task == nullptr) {
            return;
        }

        // Connect to the geometrizer that can update the widget when it tries to
        geometrize::script::GeometrizerEngine& geometrizer{m_task->getGeometrizer()};

        if(m_scriptEvaluationSucceededConnection) {
            disconnect(m_scriptEvaluationSucceededConnection);
        }
        m_scriptEvaluationSucceededConnection = connect(&geometrizer, &geometrize::script::GeometrizerEngine::signal_scriptEvaluationSucceeded, [this](const std::string& functionName, const std::string& /*code*/) {
            if(dialog::ScriptEditorWidget* editor = findEditor(functionName)) {
                editor->onScriptEvaluationSucceeded();
            }
        });

        if(m_scriptEvaluationFailedConnection) {
            disconnect(m_scriptEvaluationFailedConnection);
        }
        m_scriptEvaluationFailedConnection = connect(&geometrizer, &geometrize::script::GeometrizerEngine::signal_scriptEvaluationFailed, [this](const std::string& functionName, const std::string& /*code*/, const std::string& errorMessage) {
            if(dialog::ScriptEditorWidget* editor = findEditor(functionName)) {
                editor->onScriptEvaluationFailed(errorMessage);
            }
        });

        if(m_task) {
            setScriptModeEnabled(m_task->getPreferences().isScriptModeEnabled());

            const auto& scripts = getScripts();
            for(const auto& script : scripts) {
                m_task->getPreferences().setScript(script.first, script.second);
            }
        }
    }

    void syncUserInterface()
    {
        setScriptModeEnabled(m_task->getPreferences().isScriptModeEnabled());
        setScripts(m_task->getPreferences().getScripts());
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

    std::map<std::string, std::string> getScripts() const
    {
        std::map<std::string, std::string> m;
        for(ScriptEditorWidget* editor : m_editors) {
            m[editor->getFunctionName()] = editor->getCurrentCode();
        }
        return m;
    }


private:
    void populateUi()
    {

    }

    void buildPresetsMenuAndSelectPreset()
    {
        ui->scriptsPresetsComboBox->clear();

        // Populate the presets dropdown
        // Set the menu to first index, slots will populate the panel with the default scripts
        const QStringList presetOptions = QStringList()
                << tr("Default Scripts")
                << tr("Area Focus Scripts");

        ui->scriptsPresetsComboBox->addItems(presetOptions);
    }

    void setupPreset(const int idx)
    {
        qDeleteAll(m_editors);
        m_editors.clear();

        // Setup the script editor widgets
        const auto scripts = [idx]() -> std::map<std::string, std::string> {
            switch(idx) {
            case 0:
                return geometrize::script::getDefaultShapeMutatorScripts();
            case 1:
                return geometrize::script::getPointerAreaOfInterestShapeMutatorScripts();
            default:
                assert(0 && "Bad scripts presets index");
                return {{}};
            }
        }();

        for(const auto& item : scripts) {
            ScriptEditorWidget* editor{new ScriptEditorWidget(item.first, item.first, item.second)};
            m_editors.push_back(editor);
            connect(editor, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
                emit q->signal_scriptChanged(functionName, code);
            });
            ui->scriptEditorsContainer->addWidget(editor);
        }

        if(m_task) {
            m_task->getPreferences().setScripts(scripts);
        }
    }

    void setScriptModeEnabled(const bool enabled)
    {
        if(m_task) {
            m_task->getPreferences().setScriptModeEnabled(enabled);
        }
        ui->scriptsEnabledButton->setChecked(enabled);
        ui->scriptsPresetsComboBox->setEnabled(enabled);
        ui->rescanScriptFoldersButton->setEnabled(enabled);
        ui->resetScriptEngineButton->setEnabled(enabled);
        for(const auto& editor : m_editors) {
            editor->setEnabled(enabled);
        }
    }
    std::map<std::string, std::string> getDefaultScripts() const
    {
        std::map<std::string, std::string> m;
        for(ScriptEditorWidget* editor : m_editors) {
            m[editor->getFunctionName()] = editor->getDefaultCode();
        }
        return m;
    }

    dialog::ScriptEditorWidget* findEditor(const std::string& functionName) {
        auto it = std::find_if(m_editors.begin(), m_editors.end(), [&functionName](dialog::ScriptEditorWidget* editor) {
            return editor->getFunctionName() == functionName;
        });
        if(it != m_editors.end()) {
            return *it;
        }
        return nullptr;
    }

    void setScripts(const std::map<std::string, std::string>& scripts)
    {
        // Look for matching script editor widgets, and set their contents if found
        for(const auto& script : scripts) {
            if(dialog::ScriptEditorWidget* editor = findEditor(script.first)) {
                if(script.second.length() == 0) {
                    editor->resetCodeToDefault();
                } else {
                    editor->setCurrentCode(script.second);
                }
            }
        }
    }

    QMetaObject::Connection m_scriptEvaluationSucceededConnection{}; ///> Connection for the scripting panel to react when a script is successfully evaluated
    QMetaObject::Connection m_scriptEvaluationFailedConnection{}; ///> Connection for the scripting panel to react when a script fails to evaluate
    std::vector<ScriptEditorWidget*> m_editors;
    std::unique_ptr<Ui::ImageTaskShapeScriptingPanel> ui;
    ImageTaskShapeScriptingPanel* q;
    task::ImageTask* m_task{nullptr};
};

ImageTaskShapeScriptingPanel::ImageTaskShapeScriptingPanel(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskShapeScriptingPanel::ImageTaskShapeScriptingPanelImpl>(this)}
{
}

ImageTaskShapeScriptingPanel::~ImageTaskShapeScriptingPanel()
{
}

void ImageTaskShapeScriptingPanel::setImageTask(task::ImageTask* task)
{
    d->setImageTask(task);
}

void ImageTaskShapeScriptingPanel::syncUserInterface()
{
    d->syncUserInterface();
}

std::map<std::string, std::string> ImageTaskShapeScriptingPanel::getScripts() const
{
    return d->getScripts();
}

void ImageTaskShapeScriptingPanel::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
