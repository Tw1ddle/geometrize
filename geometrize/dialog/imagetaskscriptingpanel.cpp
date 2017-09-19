#include "imagetaskscriptingpanel.h"
#include "ui_imagetaskscriptingpanel.h"

#include <memory>
#include <string>

#include <QEvent>

#include "geometrize/shape/shapetypes.h"

#include "dialog/scripteditorwidget.h"
#include "localization/strings.h"
#include "script/scriptutil.h"

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
        q->setAttribute(Qt::WA_DeleteOnClose);
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

        connect(ui->scriptsEnabledButton, &QCheckBox::toggled, [this](const bool enabled) {
            emit q->signal_scriptingToggled(enabled);
        });
        connect(ui->resetScriptEngineButton, &QPushButton::pressed, [this]() {
            for(ScriptEditorWidget* editor : m_editors) {
                editor->resetCodeToDefault();
            }

            emit q->signal_scriptsReset();
        });
    }
    ~ImageTaskScriptingPanelImpl() = default;
    ImageTaskScriptingPanelImpl operator=(const ImageTaskScriptingPanelImpl&) = delete;
    ImageTaskScriptingPanelImpl(const ImageTaskScriptingPanelImpl&) = delete;

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

    std::vector<ScriptEditorWidget*> m_editors;
    ImageTaskScriptingPanel* q;
    std::unique_ptr<Ui::ImageTaskScriptPanel> ui;
};

ImageTaskScriptingPanel::ImageTaskScriptingPanel(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskScriptingPanel::ImageTaskScriptingPanelImpl>(this)}
{
}

ImageTaskScriptingPanel::~ImageTaskScriptingPanel()
{
}

void ImageTaskScriptingPanel::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

std::map<std::string, std::string> ImageTaskScriptingPanel::getScripts() const
{
    return d->getScripts();
}

}

}
