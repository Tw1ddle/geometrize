#pragma once

#include <memory>

#include <QPushButton>

class QString;

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The TemplateButton class is a button that opens a job template when clicked.
 */
class TemplateButton : public QPushButton
{
    Q_OBJECT

public:
    explicit TemplateButton(chaiscript::ChaiScript* const templateLoader, const QString& templateFolder);
    ~TemplateButton();

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;

    class TemplateButtonImpl;
    std::unique_ptr<TemplateButtonImpl> d;
};

}

}
