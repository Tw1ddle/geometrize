#pragma once

#include <map>
#include <memory>
#include <string>

#include <QWidget>

class QEvent;

namespace geometrize
{

namespace dialog
{
class ScriptEditorWidget;
}

}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskScriptingPanel models the UI for the script editors for controlling image task shape setup, mutation etc.
 */
class ImageTaskScriptingPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ImageTaskScriptingPanel(QWidget* parent = nullptr);
    ~ImageTaskScriptingPanel();

    /**
     * @brief getScripts Gets the map of script function names and code within this scripting panel.
     * @return A map of script function names and code within this scripting panel.
     */
    std::map<std::string, std::string> getScripts() const;

signals:
    void signal_scriptingToggled(bool enabled);
    void signal_scriptsReset();

    void signal_scriptReset(ScriptEditorWidget* editor);
    void signal_scriptApplied(ScriptEditorWidget* editor);

protected:
    void changeEvent(QEvent*) override;

private:
    class ImageTaskScriptingPanelImpl;
    std::unique_ptr<ImageTaskScriptingPanelImpl> d;
};

}

}
