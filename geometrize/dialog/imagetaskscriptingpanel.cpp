#include "imagetaskscriptingpanel.h"
#include "ui_imagetaskscriptingpanel.h"

#include <algorithm>
#include <memory>
#include <string>

#include <QEvent>

#include "dialog/scripteditorwidget.h"
#include "script/geometrizerengine.h"
#include "script/scriptutil.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace dialog
{

class ImageTaskScriptingPanel::ImageTaskScriptingPanelImpl
{
public:
    ImageTaskScriptingPanelImpl(ImageTaskScriptingPanel* pQ) : ui{std::make_unique<Ui::ImageTaskScriptPanel>()}, q{pQ}
    {
        q->setWindowFlags(Qt::Window);
        ui->setupUi(q);

        // Setup the script editor widgets
        const std::map<std::string, std::string> scriptDefaults{geometrize::script::getDefaultScripts()};
        for(const auto& item : scriptDefaults) {
            ScriptEditorWidget* editor{new ScriptEditorWidget(item.first, item.first, item.second)};
            m_editors.push_back(editor);
            connect(editor, &ScriptEditorWidget::signal_scriptChanged, [this](ScriptEditorWidget* /*self*/, const std::string& functionName, const std::string& code) {
                emit q->signal_scriptChanged(functionName, code);
            });
            ui->scriptEditorsContainer->addWidget(editor);
        }

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

        // Setup the actual actions that manipulate the image geometrization script functions
        connect(q, &geometrize::dialog::ImageTaskScriptingPanel::signal_scriptChanged, [this](const std::string& functionName, const std::string& code) {
            m_task->getPreferences().setScript(functionName, code);
        });
        connect(q, &geometrize::dialog::ImageTaskScriptingPanel::signal_scriptingToggled, [this](const bool enableScripting) {
            setScriptModeEnabled(enableScripting);
        });
        connect(ui->resetScriptEngineButton, &QPushButton::pressed, [this]() {
            m_task->getPreferences().setScripts(getScripts());
        });
    }
    ~ImageTaskScriptingPanelImpl() = default;
    ImageTaskScriptingPanelImpl operator=(const ImageTaskScriptingPanelImpl&) = delete;
    ImageTaskScriptingPanelImpl(const ImageTaskScriptingPanelImpl&) = delete;

    void setImageTask(task::ImageTask* task)
    {
        m_task = task;

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

        syncUserInterface();
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

private:
    void populateUi()
    {

    }

    void setScriptModeEnabled(const bool enabled)
    {
        m_task->getPreferences().setScriptModeEnabled(enabled);
        ui->scriptsEnabledButton->setChecked(enabled);
        ui->resetScriptEngineButton->setEnabled(enabled);
        for(const auto& editor : m_editors) {
            editor->setEnabled(enabled);
        }
    }

    std::map<std::string, std::string> getScripts() const
    {
        std::map<std::string, std::string> m;
        for(ScriptEditorWidget* editor : m_editors) {
            m[editor->getFunctionName()] = editor->getCurrentCode();
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
    std::unique_ptr<Ui::ImageTaskScriptPanel> ui;
    ImageTaskScriptingPanel* q;
    task::ImageTask* m_task{nullptr};
};

ImageTaskScriptingPanel::ImageTaskScriptingPanel(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskScriptingPanel::ImageTaskScriptingPanelImpl>(this)}
{
}

ImageTaskScriptingPanel::~ImageTaskScriptingPanel()
{
}

void ImageTaskScriptingPanel::setImageTask(task::ImageTask* task)
{
    d->setImageTask(task);
}

void ImageTaskScriptingPanel::syncUserInterface()
{
    d->syncUserInterface();
}

void ImageTaskScriptingPanel::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
