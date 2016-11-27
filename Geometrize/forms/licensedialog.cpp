#include "licensedialog.h"
#include "ui_licensedialog.h"

LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    ui->licenseTextBrowser->setText("Copyright (C) 2016 Sam Twidale. All rights reserved.");
    ui->licenseTextBrowser->append("");
    ui->licenseTextBrowser->append("This is proprietary software, provided 'as is' and 'as available', without warranty of any kind. The application provider disclaims all warranties and conditions with respect to the application.");
    ui->licenseTextBrowser->append("");
    ui->licenseTextBrowser->append("End users must not rent, lease, lend, sell, redistribute or sublicense this application.");
    ui->licenseTextBrowser->append("");
    ui->licenseTextBrowser->append("You may not copy, reverse engineer, modify or create derivative works of this application.");
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}
