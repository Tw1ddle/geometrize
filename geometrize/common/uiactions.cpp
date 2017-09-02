#include "uiactions.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QUrl>

#include "common/constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/globalpreferencesdialog.h"
#include "dialog/launchwindow.h"
#include "dialog/openurldialog.h"
#include "dialog/runscriptdialog.h"
#include "dialog/svgpreviewdialog.h"

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

void openGlobalPreferences(QWidget* parent)
{
    dialog::GlobalPreferencesDialog preferencesDialog(parent);
    preferencesDialog.exec();
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

QString openSaveRasterizedSVGPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Rasterized SVG Image"), "", QWidget::tr("Image Files (*.jpg *.jpeg *.png *.bmp)", ""));
}

QString openSaveRasterizedSVGsPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getExistingDirectory(parent, QWidget::tr("Save Rasterized SVG Images"), "");
}

QString openSaveBitmapPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Bitmap Image"), "", QWidget::tr("BMP Image (*.bmp)"));
}

QString openSaveGeometryDataPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Geometry Data"), "", QWidget::tr("JSON Data (*.json);;Text Array (*.txt)"));
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

QUrl openGetUrlDialog(QWidget* parent)
{
    dialog::OpenUrlDialog dialog(parent);
    dialog.exec();
    return dialog.getUrl();
}

QString openGetScriptDialog(QWidget* parent)
{
    dialog::RunScriptDialog dialog(parent);
    dialog.exec();
    return dialog.getScriptFilepath();
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

QString openLoadGlobalSettingsDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent, QWidget::tr("Load Global Settings"), "", QWidget::tr("JSON Global Settings File (*.json)"));
}

QString openSaveGlobalSettingsDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Global Settings"), "", QWidget::tr("JSON Global Settings File (*.json)"));
}

void openSVGPreviewPage(QWidget* parent)
{
    dialog::SvgPreviewDialog dialog(parent);
    dialog.exec();
}

}

}

}
