#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "common/constants.h"
#include "dialog/creditsdialog.h"
#include "dialog/licensedialog.h"
#include "localization/strings.h"
#include "version/versioninfo.h"

namespace geometrize
{

namespace dialog
{

AboutDialog::AboutDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    setWindowTitle(tr("About %1", "The title of the 'about' page with information about the application").arg(geometrize::strings::Strings::getApplicationName()));

    ui->nameAndVersion->setText(geometrize::strings::Strings::getApplicationName() + " " + geometrize::version::getApplicationVersionString());
    ui->appShortDescription->setText(geometrize::strings::Strings::getShortApplicationDescription());

    ui->buildInfo->setText(tr("Build Info:", "Technical information about the way the software was built follows") +
                           " " + geometrize::version::getBuildAbiName() +
                           " " + "(" + geometrize::version::getBuildDateTime() + ")");

    ui->runtimeInfo->setText(tr("Runtime Info:", "Technical information about the platform the software is running on follows") +
                             " " + geometrize::version::getBuildOperatingSystemName() +
                             " " + geometrize::version::getRuntimeCpuArchitectureName() +
                             " " + geometrize::version::getRuntimeQtVersionName() +
                             " " + geometrize::version::getRuntimeMachineHostName());

    ui->copyrightNotice->setText(geometrize::strings::Strings::getCopyrightNotice());
    ui->personalWebsiteLink->setText("<a href=\"" + geometrize::constants::DEVELOPER_WEBSITE_URL + "\">" + geometrize::constants::DEVELOPER_WEBSITE_URL + "</a>");
    ui->projectWebsiteLink->setText("<a href=\"" + geometrize::constants::PROJECT_WEBSITE_URL + "\">" + geometrize::constants::PROJECT_WEBSITE_URL + "</a>");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_closeButton_released()
{
    close();
}

void AboutDialog::on_licenseButton_released()
{
    LicenseDialog dialog(this);
    dialog.exec();
}

void AboutDialog::on_creditsButton_released()
{
    CreditsDialog dialog(this);
    dialog.exec();
}

}

}
