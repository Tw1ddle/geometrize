#pragma once

#include <QString>
#include <QPushButton>

namespace Ui
{
class TemplateButton;
}

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The TemplateButton class is a button that opens a pre-made job template when clicked.
 */
class TemplateButton : public QPushButton
{
    Q_OBJECT

public:
    explicit TemplateButton(chaiscript::ChaiScript* const templateLoader, const QString& templateFolder);
    ~TemplateButton();

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;
    void openTemplate();

    Ui::TemplateButton* ui;

    const QString m_templateFolder;
    chaiscript::ChaiScript* const m_templateLoader;
};

}

}
