#pragma once

#include <memory>

#include <QDragLeaveEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QString>

#include "manifest/templatemanifest.h"

class QEvent;

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
    virtual ~TemplateButton();

    /**
     * @brief getTemplateManifest Get the manifest data of the template this item corresponds to.
     * @return The template manifest data this item corresponds to.
     */
    TemplateManifest getTemplateManifest() const;

signals:
    void signal_templateLoaded(QString, bool);

protected:
    void changeEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;

    class TemplateButtonImpl;
    std::unique_ptr<TemplateButtonImpl> d;
};

}

}
