#include "imagejobscriptingpanel.h"
#include "ui_imagejobscriptingpanel.h"

#include <memory>
#include <string>

#include "geometrize/shape/shapetypes.h"

#include "dialog/scripteditorwidget.h"
#include "localization/strings.h"
#include "script/scriptutil.h"

namespace geometrize
{

namespace dialog
{

class ImageJobScriptingPanel::ImageJobScriptingPanelImpl
{
public:
    ImageJobScriptingPanelImpl(ImageJobScriptingPanel* pQ) : ui{std::make_unique<Ui::ImageJobScriptPanel>()}, q{pQ}
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
    ~ImageJobScriptingPanelImpl() = default;
    ImageJobScriptingPanelImpl operator=(const ImageJobScriptingPanelImpl&) = delete;
    ImageJobScriptingPanelImpl(const ImageJobScriptingPanelImpl&) = delete;

private:
    ImageJobScriptingPanel* q;
    std::unique_ptr<Ui::ImageJobScriptPanel> ui;
};

ImageJobScriptingPanel::ImageJobScriptingPanel(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageJobScriptingPanel::ImageJobScriptingPanelImpl>(this)}
{
}

ImageJobScriptingPanel::~ImageJobScriptingPanel()
{
}

}

}
