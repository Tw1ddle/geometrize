#pragma once

#include <memory>

#include <QGroupBox>
#include <QString>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ScriptEditorWidget class implements a widget for interactively editing a Chaiscript function.
 */
class ScriptEditorWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ScriptEditorWidget(const QString& title, const std::string& targetName, const std::string& defaultCode, QWidget* parent = nullptr);
    ~ScriptEditorWidget();

signals:
    /**
     * @brief signal_scriptCodeChanged Signal emitted when the code in the editor changes e.g. the user edits the code.
     * @param self The widget emitting this signal.
     * @param targetName The name of the Chaiscript global to set.
     * @param code The new Chaiscript code.
     */
    void signal_scriptCodeChanged(ScriptEditorWidget* self, const std::string& targetName, const std::string& code);

    /**
     * @brief signal_scriptCommitted Signal emitted when the user commits to a script code change e.g. presses an "apply" button.
     * @param self The widget emitting this signal.
     * @param targetName The name of the Chaiscript global to set.
     * @param code The new Chaiscript code.
     */
    void signal_scriptCommitted(ScriptEditorWidget* self, const std::string& targetName, const std::string& code);

private:
    class ScriptEditorWidgetImpl;
    std::unique_ptr<ScriptEditorWidgetImpl> d;
};

}

}
