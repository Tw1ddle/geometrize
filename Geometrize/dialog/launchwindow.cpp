#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDialog>

#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

LaunchWindow::LaunchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LaunchWindow)
{
    ui->setupUi(this);
}

LaunchWindow::~LaunchWindow()
{
    delete ui;
}

void LaunchWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{SharedApp().openQuitDialog(this)}; // TODO unsaved changes check
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void LaunchWindow::on_actionPreferences_triggered()
{
    SharedApp().openPreferences(this);
}

void LaunchWindow::on_actionClear_Recents_triggered()
{
    // TODO
}

void LaunchWindow::on_actionExit_triggered()
{
    // TODO
    const int dialogResult{SharedApp().openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            QApplication::quit();
    }
}

void LaunchWindow::on_openImageButton_clicked()
{
    const QString imagePath{SharedApp().getImagePath(this)};
    if(imagePath.length() == 0) {
        return;
    }

    const QPixmap pixmap{SharedApp().openPixmap(this, imagePath)};
    SharedApp().createImageJob(nullptr, pixmap);
}

void LaunchWindow::on_actionTutorials_triggered()
{
    SharedApp().openOnlineTutorials();
}

void LaunchWindow::on_actionSupport_triggered()
{
    SharedApp().openTechnicalSupport();
}

void LaunchWindow::on_actionAbout_triggered()
{
    SharedApp().openAboutPage(this);
}

}

}
