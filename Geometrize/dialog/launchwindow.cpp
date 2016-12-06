#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDialog>
#include <QPushButton> // TODO replace with selectable recents

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

    QPushButton* button = new QPushButton(tr("New image"));
    connect(button, SIGNAL(released()), this, SLOT(on_emptyImage_Opened()));
    ui->recentsGrid->addWidget(button);
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

// TODO add option to open an image from geometry data (draws it to a new canvas and then opens it as an image?)

void LaunchWindow::on_emptyImage_Opened()
{
    // TODO open an actual image using image opening dialog
    //SharedApp().openImage(ui->imageView->scene(), this);

    SharedApp().createImageJob(nullptr);
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
