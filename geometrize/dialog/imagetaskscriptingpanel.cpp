#include "imagetaskscriptingpanel.h"
#include "ui_imagetaskscriptingpanel.h"

#include <algorithm>
#include <memory>
#include <string>

#include <QEvent>

#include "dialog/scripteditorwidget.h"
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
            connect(editor, &ScriptEditorWidget::signal_scriptApplied, [this](ScriptEditorWidget* self, const std::string&) {
                emit q->signal_scriptApplied(self);
            });
            connect(editor, &ScriptEditorWidget::signal_scriptReset, [this](ScriptEditorWidget* self, const std::string&) {
                emit q->signal_scriptReset(self);
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
        connect(q, &geometrize::dialog::ImageTaskScriptingPanel::signal_scriptReset, [this](geometrize::dialog::ScriptEditorWidget*) {
            m_task->getPreferences().setScripts(getScripts());
        });
        connect(q, &geometrize::dialog::ImageTaskScriptingPanel::signal_scriptApplied, [this](geometrize::dialog::ScriptEditorWidget*) {
            m_task->getPreferences().setScripts(getScripts());
        });
        connect(q, &geometrize::dialog::ImageTaskScriptingPanel::signal_scriptingToggled, [this](const bool enableScripting) {
            m_task->getPreferences().setScriptModeEnabled(enableScripting);
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
        syncUserInterface();
    }

    void syncUserInterface()
    {
        ui->scriptsEnabledButton->setChecked(m_task->getPreferences().isScriptModeEnabled());
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

    std::map<std::string, std::string> getScripts() const
    {
        std::map<std::string, std::string> m;
        for(ScriptEditorWidget* editor : m_editors) {
            m[editor->getFunctionName()] = editor->getCurrentCode();
        }
        return m;
    }

    void setScripts(const std::map<std::string, std::string>& scripts)
    {
        // Look for matching script editor widgets, and set their contents if found
        for(const auto& script : scripts) {
            auto it = std::find_if(m_editors.begin(), m_editors.end(), [&s = script](dialog::ScriptEditorWidget* editor) {
                return editor->getFunctionName() == s.first;
            });

            if(it != m_editors.end()) {
                if(script.second.length() == 0) {
                    (*it)->resetCodeToDefault();
                } else {
                    (*it)->setCurrentCode(script.second);
                }
            }
        }
    }

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
