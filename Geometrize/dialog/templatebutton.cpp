#include "templatebutton.h"
#include "ui_templatebutton.h"

#include <assert.h>

#include <QMessageBox>
#include <QPixmap>

#include "chaiscript/chaiscript.hpp"
#include "formatsupport.h"
#include "script/scriptrunner.h"
#include "templatemanifest.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

TemplateButton::TemplateButton(chaiscript::ChaiScript* const templateLoader, const QString& templateFolder) :
    QPushButton(nullptr),
    ui(new Ui::TemplateButton),
    m_templateLoader{templateLoader},
    m_templateFolder{templateFolder}
{
    ui->setupUi(this);

    const QString firstImageFile{QString::fromStdString(util::getFirstFileWithExtensions(m_templateFolder.toStdString(), format::getSupportedImageFileExtensions(false)))};
    const TemplateManifest manifest(util::getFirstFileWithExtensions(m_templateFolder.toStdString(), format::getSupportedTemplateManifestFileExtensions()));

    setToolTip(QString::fromStdString(manifest.getName() + " (" + manifest.getLicense() + ")"));
    ui->titleLabel->setText(QString::fromStdString(manifest.getName()));

    if(!firstImageFile.isEmpty()) {
        const QPixmap thumbnail(firstImageFile);
        if(!thumbnail.isNull()) {
            const QSize size{ui->imageLabel->size()};
            ui->imageLabel->setPixmap(thumbnail.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
    }

    connect(this, &TemplateButton::clicked, [this]() {
        const std::vector<std::string> scripts{util::getScriptsForPath(m_templateFolder.toStdString())};

        if(scripts.empty()) {
            assert(0 && "Could not find script for template");
            return;
        }

        const std::string script{util::readFileAsString(scripts.front())};
        m_templateLoader->set_global(chaiscript::var(m_templateFolder.toStdString()), "templateDirectory");
        geometrize::script::runScript(script, *m_templateLoader, nullptr);
    });
}

TemplateButton::~TemplateButton()
{
    delete ui;
}

}

}
