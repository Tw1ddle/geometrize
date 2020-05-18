#include "uiactions.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPoint>
#include <QScreen>
#include <QStyle>
#include <QUrl>
#include <QWidget>

#include "common/constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/globalpreferencesdialog.h"
#include "dialog/launchwindow.h"
#include "dialog/openurldialog.h"
#include "dialog/runscriptdialog.h"
#include "dialog/taskqueuewindow.h"
#include "dialog/welcomewindow.h"

namespace geometrize
{

namespace common
{

namespace ui
{

const QScreen* getFirstScreenContainingCursor()
{
    const QPoint globalCursorPos = QCursor::pos();
    const QScreen* mouseScreen = QGuiApplication::screenAt(globalCursorPos);
    return mouseScreen;
}

void centerWidgetOnScreen(QWidget& widget, const QScreen& screen)
{
    widget.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, widget.size(), screen.availableGeometry()));
}

void openLaunchWindow()
{
    dialog::LaunchWindow* launcher{new dialog::LaunchWindow()};
    launcher->show();
    if(const auto mouseScreen = geometrize::common::ui::getFirstScreenContainingCursor()) {
        geometrize::common::ui::centerWidgetOnScreen(*launcher, *mouseScreen);
    }
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

void openTaskQueueWindow()
{
    auto taskQueueWindow = new dialog::TaskQueueWindow();
    taskQueueWindow->show();
}

void openWelcomePage()
{
    dialog::WelcomeWindow* window = new dialog::WelcomeWindow();
    window->show();
    if(const auto mouseScreen = geometrize::common::ui::getFirstScreenContainingCursor()) {
        geometrize::common::ui::centerWidgetOnScreen(*window, *mouseScreen);
    }
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

QString openGenericFolderPickerDialog(QWidget* parent)
{
    return QFileDialog::getExistingDirectory(parent, QWidget::tr("Select Folder", "Title on a dialog that allows the user to select a folder"));
}

QString openImagePathPickerDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent,
                                        QWidget::tr("Open Image", "Title on a dialog that allows the user to open an image file"),
                                        "",
                                        QWidget::tr("Image Files (*.jpg *.jpeg *.png *.bmp)", "List of supported image file formats. The text in the parentheses must not be changed"));
}

QString openSaveImagePathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save Image", "Title on a dialog that allows the user to save an image file"),
                                        "",
                                        QWidget::tr("JPEG Image (*.jpg *.jpeg);;PNG Image (*.png);;BMP Image (*.bmp);;PPM Image (*.ppm)", "List of supported image file formats. Semicolons and text in the parentheses must not be changed"));
}

QString openSaveSVGPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save SVG Image", "Title on a dialog that allows the user to save an SVG (scalable vector graphics) image file"),
                                        "",
                                        QWidget::tr("SVG Image (*.svg)", "List of supported vector-based image formats. The text in the parentheses must not be changed"));
}

QString openSaveRasterizedSVGPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save Image", "Title on a dialog that allows the user to save an image file"),
                                        "",
                                        QWidget::tr("Image Files (*.jpg *.jpeg *.png *.bmp)", "List of supported image file formats. The text in the parentheses must not be changed"));
}

QString openSaveRasterizedSVGsPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getExistingDirectory(parent,
                                             QWidget::tr("Save Images", "Title on a dialog that allows the user to save image files"));
}

QString openSaveGeometryDataPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save Geometry Data", "Title on a dialog that allows the user to save geometric primitive data"),
                                        "",
                                        QWidget::tr("JSON Data (*.json);;Text Array (*.txt)", "List of supported data file formats. Semicolons and text in the parentheses must not be changed"));
}

QString openSaveGIFPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save Animated GIF", "Title on a dialog that allows the user to save an animated GIF file"),
                                        "",
                                        QWidget::tr("GIF Animation (*.gif)", "List of supported GIF file formats. The text in the parentheses must not be changed"));
}

QString openSaveCanvasPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save HTML5 Webpage", "Title on a dialog that allows the user to save a HTML5-based webpage"),
                                        "",
                                        QWidget::tr("HTML Webpage (*.html)", "List of supported webpage formats. The text in the parentheses must not be changed"));
}

QString openSaveWebGLPathPickerDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save WebGL Webpage", "Title on a dialog that allows the user to save a WebGL-based webpage"),
                                        "",
                                        QWidget::tr("HTML Webpage (*.html)", "List of supported webpage formats. The text in the parentheses must not be changed"));
}

QUrl openGetUrlDialog(QWidget* parent)
{
    dialog::OpenUrlDialog dialog(parent);
    if(dialog.exec() == QDialog::Accepted) {
        return dialog.getUrl();
    }
    return QUrl();
}

QString openGetScriptDialog(QWidget* parent)
{
    dialog::RunScriptDialog dialog(parent);
    dialog.exec();
    return dialog.getScriptFilepath();
}

QString openSelectScriptDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent,
                                        QWidget::tr("Select Script", "Title on a dialog that allows the user to select a script file"),
                                        "",
                                        QWidget::tr("ChaiScript Files (*.chai)", "List of supported script file formats. The text in the parentheses must not be changed"));
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

QString openLoadImageTaskSettingsDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent,
                                        QWidget::tr("Load Settings", "Title on a dialog that allows the user to select a settings file"),
                                        "",
                                        QWidget::tr("JSON Settings File (*.json)", "List of supported settings file formats. The text in the parentheses must not be changed"));
}

QString openSaveImageTaskSettingsDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save Settings", "Title on a dialog that allows the user to save a settings file"),
                                        "",
                                        QWidget::tr("JSON Settings File (*.json)", "List of supported settings file formats. The text in the parentheses must not be changed"));
}

QString openLoadGlobalSettingsDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent,
                                        QWidget::tr("Load Settings", "Title on a dialog that allows the user to select a settings file"),
                                        "",
                                        QWidget::tr("JSON Settings File (*.json)", "List of supported settings file formats. The text in the parentheses must not be changed"));
}

QString openSaveGlobalSettingsDialog(QWidget* parent)
{
    return QFileDialog::getSaveFileName(parent,
                                        QWidget::tr("Save Settings", "Title on a dialog that allows the user to save a settings file"),
                                        "",
                                        QWidget::tr("JSON Settings File (*.json)", "List of supported settings file formats. The text in the parentheses must not be changed"));
}

QString openTargetImagePickerDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent,
                                        QWidget::tr("Select Image", "Title on a dialog that allows the user to select an image file"),
                                        "",
                                        QWidget::tr("Image Files (*.jpg *.jpeg *.png *.bmp)", "List of supported image file formats. The text in the parentheses must not be changed"));
}

}

}

}
