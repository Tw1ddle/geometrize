#include "templatebutton.h"
#include "ui_templatebutton.h"

#include <assert.h>

#include <QMessageBox>
#include <QPixmap>

#include "chaiscript/chaiscript.hpp"
#include "formatsupport.h"
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

    setToolTip(firstImageFile);
    ui->titleLabel->setText(firstImageFile);

    if(!firstImageFile.isEmpty()) {
        const QPixmap thumbnail(firstImageFile);
        if(!thumbnail.isNull()) {
            ui->imageLabel->setPixmap(thumbnail);
        }
    }

    connect(this, &TemplateButton::clicked, [this]() {
        const std::vector<std::string> scripts{util::getScriptsForPath(m_templateFolder.toStdString())};

        if(scripts.empty()) {
            assert(0 && "Could not find script for template");
            return;
        }

        const std::string script{util::readFileAsString(scripts.front())};

        try {
            m_templateLoader->set_global(chaiscript::var(m_templateFolder.toStdString()), "templateDirectory");
            m_templateLoader->eval(script);
        } catch (const std::string& s) {
            QMessageBox::warning(nullptr, QString::fromStdString(s), QString::fromStdString(s));
        } catch (const std::exception& e) {
          // This is the one what will be called in the specific throw() above
             QMessageBox::warning(nullptr, QString(e.what()), QString(e.what()));
        } catch (...) {
            QMessageBox::warning(nullptr, "Evaluation failed", "Failed to evaluate template script");
        }
    });
}

TemplateButton::~TemplateButton()
{
    delete ui;
}

}

}
