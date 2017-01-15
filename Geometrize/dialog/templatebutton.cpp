#include "templatebutton.h"
#include "ui_templatebutton.h"

#include <assert.h>

#include <QContextMenuEvent>
#include <QMessageBox>
#include <QMenu>
#include <QPixmap>
#include <QString>

#include "chaiscript/chaiscript.hpp"
#include "formatsupport.h"
#include "script/scriptrunner.h"
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
        m_templateFolder{templateFolder}
    {
        ui->setupUi(q);

        const QString firstImageFile{QString::fromStdString(util::getFirstFileWithExtensions(m_templateFolder.toStdString(), format::getSupportedImageFileExtensions(false)))};
        const TemplateManifest manifest(util::getFirstFileWithExtensions(m_templateFolder.toStdString(), format::getSupportedTemplateManifestFileExtensions()));

        q->setToolTip(QString::fromStdString(manifest.getName() + " (" + manifest.getLicense() + ")"));

        if(!firstImageFile.isEmpty()) {
            const QPixmap thumbnail(firstImageFile);
            if(!thumbnail.isNull()) {
                const QSize size{170, 150};
                ui->imageLabel->setPixmap(thumbnail.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            }
        }

        ui->titleLabel->setText(QString::fromStdString(manifest.getName()));

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
        const std::vector<std::string> scripts{util::getScriptsForPath(m_templateFolder.toStdString())};

        if(scripts.empty()) {
            assert(0 && "Could not find script for template");
            return;
        }

        const std::string script{util::readFileAsString(scripts.front())};
        m_templateLoader->set_global(chaiscript::var(m_templateFolder.toStdString()), "templateDirectory");
        geometrize::script::runScript(script, *m_templateLoader, nullptr);
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

private:
    chaiscript::ChaiScript* const m_templateLoader;
    const QString m_templateFolder;
    Ui::TemplateButton* ui;
    TemplateButton* q;
};

TemplateButton::TemplateButton(chaiscript::ChaiScript* const templateLoader, const QString& templateFolder) :
    QPushButton(nullptr),
    d{std::make_unique<TemplateButton::TemplateButtonImpl>(this, templateLoader, templateFolder)}
{
}

TemplateButton::~TemplateButton()
{
}

void TemplateButton::contextMenuEvent(QContextMenuEvent* e)
{
    d->showContextMenu(e);
}

}

}
