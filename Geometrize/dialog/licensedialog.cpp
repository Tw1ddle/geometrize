#include "licensedialog.h"
#include "ui_licensedialog.h"

#include <assert.h>

#include <QByteArray>
#include <QFile>

namespace geometrize
{

namespace dialog
{

LicenseDialog::LicenseDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    const auto readLicenseFile = [](const QString& resourcePath) -> QString {
        QFile file{resourcePath};
        if(!file.exists()) {
            assert(0 && "Bad license file path");
            return "";
        }

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            assert(0 && "Failed to open license file for reading");
            return "";
        }

        const QByteArray data{file.readAll()};

        if(file.error()) {
            assert(0 && "Failed to read license file");
            return "";
        }

        QString text{QString::fromUtf8(data)};
        if(text.toUtf8() != data) {
            assert(0 && "Possible conversion failure reading license file");
            return "";
        }

        return text;
    };

    const auto setLicenseText{[&readLicenseFile](QTextBrowser* textBrowser, const QString& licenseFilePath) {
        textBrowser->setText(readLicenseFile(":/licenses/ " + licenseFilePath));
    }};

    setLicenseText(ui->geometrizeLicense, "geometrize_license.txt");
    setLicenseText(ui->geometrizeLibraryLicense, "geometrize_library_license.txt");
    setLicenseText(ui->qtLicense, "qt_license.txt");
    setLicenseText(ui->chaiScriptLicense, "chaiscript_license.txt");
    setLicenseText(ui->cerealLicense, "cereal_license.txt");
    setLicenseText(ui->rapidJSONLicense, "rapidjson_license.txt");
    setLicenseText(ui->rapidXmlLicense, "rapidxml_license.txt");
    setLicenseText(ui->qtGifImageLicense, "qtgifimage_license.txt");
    setLicenseText(ui->GIFLIBLicense, "giflib_license.txt");
    setLicenseText(ui->qtGoogleAnalyticsLicense, "qt_google_analytics_license.txt");
    setLicenseText(ui->libsimdppLicense, "libsimdpp_license.txt");
    setLicenseText(ui->flowLayoutLicense, "flowlayout_license.txt");
    setLicenseText(ui->elidedLabelLicense, "elidedlabel_license.txt");
    setLicenseText(ui->iconAssetsLicense, "fatcow_icon_license.txt");
    setLicenseText(ui->templateAssetsLicense, "template_assets_license.txt");
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}

}

}
