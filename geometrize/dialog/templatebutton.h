#pragma once

#include <memory>

#include <QPushButton>

#include "manifest/templatemanifest.h"

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
 * @brief The TemplateButton class is a button that opens a task template when clicked.
 */
class TemplateButton : public QPushButton
{
    Q_OBJECT

public:
    explicit TemplateButton(chaiscript::ChaiScript* const templateLoader, const QString& templateFolder);
    ~TemplateButton();

    /**
     * @brief getTemplateManifest Get the manifest data of the template this item corresponds to.
     * @return The template manifest data this item corresponds to.
     */
    TemplateManifest getTemplateManifest() const;

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;

    class TemplateButtonImpl;
    std::unique_ptr<TemplateButtonImpl> d;
};

}

}
