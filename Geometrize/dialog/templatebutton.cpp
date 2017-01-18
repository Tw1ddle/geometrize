#include "templatebutton.h"
#include "ui_templatebutton.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QPixmap>
#include <QString>

#include "chaiscript/chaiscript.hpp"
#include "formatsupport.h"
#include "job/jobutil.h"
#include "templatemanifest.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

class TemplateButton::TemplateButtonImpl
{
public:
    TemplateButtonImpl(TemplateButton* pQ, chaiscript::ChaiScript* const templateLoader, const QString& templateFolder) :
        ui(new Ui::TemplateButton),
        q{pQ},
        m_templateLoader{templateLoader},
        m_templateFolder{templateFolder},
        m_manifest{util::getTemplateManifest(m_templateFolder.toStdString())}
    {
        ui->setupUi(q);

        const QString firstImageFile{QString::fromStdString(util::getFirstFileWithExtensions(m_templateFolder.toStdString(), format::getSupportedImageFileExtensions(false)))};

        q->setToolTip(QString::fromStdString(m_manifest.getName() + " (" + m_manifest.getLicense() + ")"));

        if(!firstImageFile.isEmpty()) {
            const QPixmap thumbnail(firstImageFile);
            if(!thumbnail.isNull()) {
                const QSize size{180, 150};
                ui->imageLabel->setPixmap(thumbnail.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            }
        }

        ui->titleLabel->setText(QString::fromStdString(m_manifest.getName()));

        q->connect(q, &TemplateButton::clicked, [this]() {
            openTemplate();
        });
    }

    ~TemplateButtonImpl()
    {
        delete ui;
    }

    TemplateButtonImpl operator=(const TemplateButtonImpl&) = delete;
    TemplateButtonImpl(const TemplateButtonImpl&) = delete;

    void openTemplate()
    {
        geometrize::util::openTemplate(*m_templateLoader, m_templateFolder.toStdString());
    }

    void revealTemplateInExplorer()
    {
        geometrize::util::openInDefaultApplication(m_templateFolder.toStdString());
    }

    void showContextMenu(QContextMenuEvent* e)
    {
        QMenu itemContextMenu;

        QAction openAction(tr("Open"));
        itemContextMenu.addAction(&openAction);
        connect(&openAction, &QAction::triggered, [this]() {
            openTemplate();
        });

        QAction openInDefaultViewer(tr("Reveal in explorer"));
        itemContextMenu.addAction(&openInDefaultViewer);
        connect(&openInDefaultViewer, &QAction::triggered, [this]() {
            revealTemplateInExplorer();
        });

        itemContextMenu.exec(e->globalPos());
    }

    TemplateManifest getTemplateManifest() const
    {
        return m_manifest;
    }

private:
    chaiscript::ChaiScript* const m_templateLoader;
    const QString m_templateFolder;
    const TemplateManifest m_manifest;
    Ui::TemplateButton* ui;
    TemplateButton* q;
};

TemplateButton::TemplateButton(chaiscript::ChaiScript* const templateLoader, const QString& templateFolder) :
    QPushButton(nullptr),
    d{std::make_unique<TemplateButton::TemplateButtonImpl>(this, templateLoader, templateFolder)}
{
}

void TemplateButton::contextMenuEvent(QContextMenuEvent* e)
{
    d->showContextMenu(e);
}

TemplateManifest TemplateButton::getTemplateManifest() const
{
    return d->getTemplateManifest();
}

}

}
