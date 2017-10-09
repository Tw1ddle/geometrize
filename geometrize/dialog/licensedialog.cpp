#include "licensedialog.h"
#include "ui_licensedialog.h"

#include <cassert>

#include <QByteArray>
#include <QEvent>
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
    populateUi();

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
        textBrowser->setText(readLicenseFile(":/licenses/" + licenseFilePath));
    }};

#ifdef MAC_APPSTORE_LICENSE
    // Special license for Mac appstore builds
    setLicenseText(ui->geometrizeLicense, QLatin1String("geometrize_mac_appstore_build_license.txt"));
#else
    setLicenseText(ui->geometrizeLicense, QLatin1String("geometrize_license.txt"));
#endif

    setLicenseText(ui->geometrizeLibraryLicense, QLatin1String("geometrize_library_license.txt"));
    setLicenseText(ui->qtLicense, QLatin1String("qt_license.txt"));
    setLicenseText(ui->chaiScriptLicense, QLatin1String("chaiscript_license.txt"));
    setLicenseText(ui->chaiScriptExtrasLicense, QLatin1String("chaiscript_extras_license.txt"));
    setLicenseText(ui->cerealLicense, QLatin1String("cereal_license.txt"));
    setLicenseText(ui->rapidJSONLicense, QLatin1String("rapidjson_license.txt"));
    setLicenseText(ui->rapidXmlLicense, QLatin1String("rapidxml_license.txt"));
    setLicenseText(ui->qtGifImageLicense, QLatin1String("qtgifimage_license.txt"));
    setLicenseText(ui->GIFLIBLicense, QLatin1String("giflib_license.txt"));
    setLicenseText(ui->qtGoogleAnalyticsLicense, QLatin1String("qt_google_analytics_license.txt"));
    setLicenseText(ui->flowLayoutLicense, QLatin1String("flowlayout_license.txt"));
    setLicenseText(ui->elidedLabelLicense, QLatin1String("elidedlabel_license.txt"));
    setLicenseText(ui->iconAssetsLicense, QLatin1String("fatcow_icon_license.txt"));
    setLicenseText(ui->templateAssetsLicense, QLatin1String("template_assets_license.txt"));
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}

void LicenseDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        populateUi();
    }
    QDialog::changeEvent(event);
}

void LicenseDialog::populateUi()
{

}

}

}
