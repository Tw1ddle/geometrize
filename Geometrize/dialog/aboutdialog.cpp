#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QBitmap>
#include <QPixmap>

#include "constants.h"
#include "creditsdialog.h"
#include "licensedialog.h"
#include "versioninfo.h"

namespace geometrize
{

namespace dialog
{

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    setWindowTitle(tr("About %1", "The title of the 'about' page with information about the application").arg(geometrize::constants::Strings::getApplicationName()));

    ui->nameAndVersion->setText(geometrize::constants::Strings::getApplicationName() + " " + geometrize::version::getApplicationVersionString());
    ui->appShortDescription->setText(geometrize::constants::Strings::getApplicationDescription());

    ui->buildInfo->setText(tr("Build Info:", "Technical information about the way the software was built follows") +
                           " " + geometrize::version::getBuildAbiName() +
                           " " + "(" + geometrize::version::getBuildDateTime() + ")");

    ui->runtimeInfo->setText(tr("Runtime Info:", "Technical information about the platform the software is running on follows") +
                             " " + geometrize::version::getBuildOperatingSystemName() +
                             " " + geometrize::version::getRuntimeOperatingSystemVersionName() +
                             " " + geometrize::version::getRuntimeCpuArchitectureName() +
                             " " + geometrize::version::getRuntimeQtVersionName() +
                             " " + geometrize::version::getRuntimeMachineHostName());

    ui->copyrightNotice->setText(geometrize::constants::Strings::getCopyrightNotice());
    ui->websiteLink->setText("<a href=\"" + geometrize::constants::DEVELOPER_WEBSITE_URL + "\">" + geometrize::constants::DEVELOPER_WEBSITE_URL + "</a>");
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
