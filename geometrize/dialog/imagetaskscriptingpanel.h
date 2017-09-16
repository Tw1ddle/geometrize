#pragma once

#include <memory>

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

signals:
    void scriptingEnabledToggled(bool enabled);

    void setupScriptChangeApplied(ScriptEditorWidget* editor);
    void mutationScriptChangeApplied(ScriptEditorWidget* editor);

protected:
    void changeEvent(QEvent*) override;

private:
    class ImageTaskScriptingPanelImpl;
    std::unique_ptr<ImageTaskScriptingPanelImpl> d;
};

}

}
