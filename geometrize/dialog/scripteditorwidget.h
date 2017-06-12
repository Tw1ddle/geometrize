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
    explicit ScriptEditorWidget(const QString& title, QWidget* parent = nullptr);
    ~ScriptEditorWidget();

private:
    class ScriptEditorWidgetImpl;
    std::unique_ptr<ScriptEditorWidgetImpl> d;
};

}

}
