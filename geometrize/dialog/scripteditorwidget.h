#pragma once

#include <memory>

#include <QGroupBox>
#include <QString>

class QEvent;

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
    explicit ScriptEditorWidget(const std::string& title, const std::string& functionName, const std::string& defaultCode, QWidget* parent = nullptr);
    ~ScriptEditorWidget();

signals:
    /**
     * @brief signal_scriptApplied Signal emitted when the user commits to a script code change e.g. presses an "apply" button.
     * @param self The widget emitting this signal.
     * @param code The new Chaiscript code.
     */
    void signal_scriptApplied(ScriptEditorWidget* self, const std::string& code);

    /**
     * @brief signal_scriptReset Signal emitted when the user hits the reset button, expecting the script to be reset to some default code.
     * @param self The widget emitting this signal.
     * @param code The default Chaiscript code.
     */
    void signal_scriptReset(ScriptEditorWidget* self, const std::string& code);

protected:
    void changeEvent(QEvent*) override;

private:
    class ScriptEditorWidgetImpl;
    std::unique_ptr<ScriptEditorWidgetImpl> d;
};

}

}
