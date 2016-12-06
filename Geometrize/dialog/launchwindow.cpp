#include "launchwindow.h"
#include "ui_launchwindow.h"

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

void LaunchWindow::on_emptyImage_Opened()
{
    SharedApp().createImageJob(nullptr);
}

void LaunchWindow::on_actionImage_triggered()
{
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
