#include "uiactions.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QUrl>

#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/globalpreferencestabdialog.h"
#include "dialog/launchwindow.h"
#include "dialog/openurldialog.h"
#include "dialog/quitdialog.h"
#include "dialog/runscriptdialog.h"

namespace geometrize
{

namespace common
{

namespace ui
{

void openLaunchWindow()
{
    dialog::LaunchWindow launcher;
    launcher.show();
}

bool isLaunchWindowOpen()
{
    return dialog::LaunchWindow::getTopLevelLaunchWindows().size() > 0;
}

void bringLaunchWindowToFront()
{
    std::vector<dialog::LaunchWindow*> launchWindows{dialog::LaunchWindow::getTopLevelLaunchWindows()};

    if(launchWindows.empty()) {
        return;
    }

    dialog::LaunchWindow* firstLauncher{launchWindows.front()};
    firstLauncher->setWindowState(firstLauncher->windowState() & ~Qt::WindowMinimized);
    firstLauncher->show();
    firstLauncher->raise();
    QApplication::setActiveWindow(firstLauncher);
}

void openAboutPage(QWidget* parent)
{
    dialog::AboutDialog dialog(parent);
    dialog.exec();
}

void openPreferences(QWidget* parent)
{
    dialog::GlobalPreferencesTabDialog preferencesTabDialog(parent);
    preferencesTabDialog.exec();
}

int openQuitDialog(QWidget* parent)
{
    dialog::QuitDialog dialog(parent);
    dialog.exec();
    return dialog.result();
}

QString openImagePathPickerDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent, QWidget::tr("Open Image"), "", QWidget::tr("Image Files (*.jpg *.jpeg *.png *.bmp)", ""));
}

QString openSaveImagePathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Image"), "", QWidget::tr("JPEG Image (*.jpg *.jpeg);;PNG Image (*.png);;BMP Image (*.bmp);;PPM Image (*.ppm)"));
}

QString openSaveSVGPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save SVG Image"), "", QWidget::tr("SVG Image (*.svg)"));
}

QString openSaveBitmapPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Bitmap Image"), "", QWidget::tr("BMP Image (*.bmp)"));
}

QString openSaveGeometryDataPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Geometry Data"), "", QWidget::tr("JSON Data (*.json);;XML Data (*.xml);;Text Array (*.txt)"));
}

QString openSaveGIFPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Animated GIF"), "", QWidget::tr("GIF Animation (*.gif)"));
}

QString openSaveCanvasAnimationPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Canvas Animation"), "", QWidget::tr("HTML Webpage (*.html)"));
}

QString openSaveWebGLPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save WebGL Animation"), "", QWidget::tr("HTML Webpage (*.html)"));
}

QString openSaveRawImageDataPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Raw Image Data"));
}

QUrl openGetUrlDialog(QWidget* parent)
{
    dialog::OpenUrlDialog dialog(parent);
    dialog.exec();
    return dialog.getUrl();
}

QPair<QString, geometrize::script::ScriptOptions> openGetScriptDialog(QWidget* parent)
{
    dialog::RunScriptDialog dialog(parent);
    dialog.exec();
    return QPair<QString, geometrize::script::ScriptOptions>(dialog.getScriptFilepath(), dialog.getScriptOptions());
}

QString openSelectScriptDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent, QWidget::tr("Select Script"), "", QWidget::tr("ChaiScript Files (*.chai)", ""));
}

void openTechnicalSupport()
{
    QDesktopServices::openUrl(QUrl(geometrize::constants::TECHNICAL_SUPPORT_URL));
}

void openOnlineTutorials()
{
    QDesktopServices::openUrl(QUrl(geometrize::constants::VIDEO_TUTORIAL_URL));
}

void openMoreResourcesPage()
{
    QDesktopServices::openUrl(QUrl(geometrize::constants::MORE_RESOURCES_URL));
}

QString openLoadImageJobSettingsDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent, QWidget::tr("Load Image Job Settings"), "", QWidget::tr("JSON Image Job Settings File (*.json)"));
}

QString openSaveImageJobSettingsDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Image Job Settings"), "", QWidget::tr("JSON Image Job Settings File (*.json)"));
}

}

}

}
