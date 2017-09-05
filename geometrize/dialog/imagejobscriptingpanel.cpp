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

    void setScriptingModeEnabled(const bool enabled)
    {
        //m_job->getPreferences().setScriptModeEnabled(enabled);

        //if(enabled) {
        //    activateScriptedShapeMutation();
        //} else {
        //    activateLibraryShapeMutation();
        //}
    }

    /**
     * @brief activateLibraryShapeMutation Uses the Geometrize library's built-in implementation for mutating shapes.
     */
    void activateLibraryShapeMutation()
    {
        //m_job->getShapeMutator().setDefaults();
    }

    /**
     * @brief activateScriptedShapeMutation Uses the custom scripted implementation for mutating shapes.
     */
    void activateScriptedShapeMutation()
    {
        //m_mutationRules.setupScripts(m_job->getShapeMutator(), {});
    }

    /**
     * @brief setScriptFunction Adds the given script function to the engine.
     * @param name The name of the function to add.
     * @param code The code that defines the function.
     */
    void setScriptFunction(const std::string& name, const std::string& code)
    {
        //std::map<std::string, std::string> m;
        //m[name] = code;
        //m_job->getShapeMutationRules().setupScripts(m_job->getShapeMutator(), m);
    }

    void resetShapeScriptEngine()
    {
        // TODO reset the text of all the script boxes back to defaults
        //m_job->resetShapeScriptEngine();
    }

    void setupScriptEditPanels()
    {
        //for(const geometrize::ShapeTypes type : geometrize::allShapes) {
        //    ScriptEditorWidget* editor{new ScriptEditorWidget(geometrize::strings::Strings::getShapeTypeNamePlural(type), "TODO", "TODO")};

        //    connect(editor, &ScriptEditorWidget::signal_scriptCommitted, [this](ScriptEditorWidget* self, const std::string& targetName, const std::string& scriptCode) {
                // TODO validate code?
        //        m_scriptChanges.push_back(std::make_pair(targetName, scriptCode));
        //    });
        //    connect(editor, &ScriptEditorWidget::signal_scriptCodeChanged, [this](ScriptEditorWidget* self, const std::string& targetName, const std::string& scriptCode) {
                // TODO validate and indicate if it's wrong?
        //    });
        //}
    }

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
