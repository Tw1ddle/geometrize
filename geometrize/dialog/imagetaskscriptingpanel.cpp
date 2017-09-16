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
            connect(editor, &ScriptEditorWidget::signal_scriptApplied, [this](ScriptEditorWidget* self, const std::string& scriptCode) {

            });
            connect(editor, &ScriptEditorWidget::signal_scriptReset, [this](ScriptEditorWidget* self, const std::string& scriptCode) {

            });
            ui->scriptEditorsContainer->addWidget(editor);
        }
    }
    ~ImageTaskScriptingPanelImpl() = default;
    ImageTaskScriptingPanelImpl operator=(const ImageTaskScriptingPanelImpl&) = delete;
    ImageTaskScriptingPanelImpl(const ImageTaskScriptingPanelImpl&) = delete;

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {

    }

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

}

}
