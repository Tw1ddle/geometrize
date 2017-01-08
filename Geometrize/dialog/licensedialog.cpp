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

    ui->geometrizeLicense->setText(readLicenseFile(":/licenses/geometrize_license.txt"));
    ui->geometrizerLicense->setText(readLicenseFile(":/licenses/geometrizer_license.txt"));
    ui->qtLicense->setText(readLicenseFile(":/licenses/qt_license.txt"));
    ui->chaiScriptLicense->setText(readLicenseFile(":/licenses/chaiscript_license.txt"));
    ui->cerealLicense->setText(readLicenseFile(":/licenses/cereal_license.txt"));
    ui->rapidJSONLicense->setText(readLicenseFile(":/licenses/rapidjson_license.txt"));
    ui->rapidXmlLicense->setText(readLicenseFile(":/licenses/rapidxml_license.txt"));
    ui->flowLayoutLicense->setText(readLicenseFile(":/licenses/flowlayout_license.txt"));
    ui->iconAssetsLicense->setText(readLicenseFile(":/licenses/fatcow_icon_license.txt"));
    ui->templateAssetsLicense->setText(readLicenseFile(":/licenses/template_assets_license.txt"));
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}

}

}
