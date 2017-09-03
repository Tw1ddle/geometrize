#include "imagejobscriptingpanel.h"
#include "ui_imagejobscriptingpanel.h"

#include <memory>

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
