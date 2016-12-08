#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDialog>
#include <QMap>

#include "dialog/recentitembutton.h"
#include "recentitems.h"
#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl()
    {
    }

    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

private:

};

LaunchWindow::LaunchWindow(QWidget *parent) :
    QMainWindow(parent),
    d{std::make_unique<LaunchWindow::LaunchWindowImpl>()},
    ui(new Ui::LaunchWindow)
{
    ui->setupUi(this);
}

LaunchWindow::~LaunchWindow()
{
    delete ui;
}

void LaunchWindow::dragEnterEvent(QDragEnterEvent* event)
{
    // TODO
}

void LaunchWindow::dropEvent(QDropEvent* event)
{
    // TODO
}

void LaunchWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{SharedApp::get().openQuitDialog(this)}; // TODO unsaved changes check
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void LaunchWindow::on_actionPreferences_triggered()
{
    SharedApp::get().openPreferences(this);
}

void LaunchWindow::on_actionClear_Recents_triggered()
{
    SharedApp::get().getRecentFiles().clear();
}

void LaunchWindow::on_actionExit_triggered()
{
    // TODO
    const int dialogResult{SharedApp::get().openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            QApplication::quit();
    }
}

void LaunchWindow::on_openImageButton_clicked()
{
    const QString imagePath{SharedApp::get().getImagePath(this)};
    if(imagePath.length() == 0) {
        return;
    }

    const QPixmap pixmap{SharedApp::get().openPixmap(this, imagePath)};
    SharedApp::get().createImageJob(nullptr, pixmap);
}

void LaunchWindow::on_actionTutorials_triggered()
{
    SharedApp::get().openOnlineTutorials();
}

void LaunchWindow::on_actionSupport_triggered()
{
    SharedApp::get().openTechnicalSupport();
}

void LaunchWindow::on_actionAbout_triggered()
{
    SharedApp::get().openAboutPage(this);
}

}

}
