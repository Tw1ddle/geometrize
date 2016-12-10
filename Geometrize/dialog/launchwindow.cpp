#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QMap>

#include "dialog/itembutton.h"
#include "recentitems.h"
#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

void createImageJob(QWidget* parent, const QString& imagePath)
{
    SharedApp::get().createImageJob(parent, QPixmap(imagePath));
}

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl(LaunchWindow* pQ, Ui::LaunchWindow* pUi) : q{pQ}, ui{pUi}
    {
        ui->setupUi(q);

        connect(ui->recentsList, &RecentJobsList::itemActivated, [this](QListWidgetItem* item) {
            qDebug() << "Item activated " << item->text();

            // TODO deal with bad paths, use data not text
            createImageJob(q, item->text());
        });

        ui->recentsList->setRecentItems(&SharedApp::get().getRecentFiles());
    }

    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

private:
    LaunchWindow* q;
    Ui::LaunchWindow* ui;
};

LaunchWindow::LaunchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LaunchWindow),
    d{std::make_unique<LaunchWindow::LaunchWindowImpl>(this, ui)}
{

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
    createImageJob(this, imagePath);
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
