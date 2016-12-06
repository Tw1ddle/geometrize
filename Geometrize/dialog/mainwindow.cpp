#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton> // TODO replace with selectable recents

#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* button = new QPushButton(tr("New image"));
    connect(button, SIGNAL(released()), this, SLOT(on_emptyImage_Opened()));
    ui->recentsGrid->addWidget(button);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_emptyImage_Opened()
{
    geometrize::SharedApp().createImageJob(this);
}

}

}
