#include "uiactions.h"

#include <assert.h>

#include <QDesktopServices>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QUrl>

// TODO remove
#include "geometrize/bitmap/bitmapdata.h"
#include "geometrize/bitmap/rgba.h"

#include "common/sharedapp.h"
#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/imagejobwindow.h"
#include "dialog/launchwindow.h"
#include "dialog/openurldialog.h"
#include "dialog/globalpreferencestabdialog.h"
#include "dialog/recentjobslist.h"
#include "dialog/quitdialog.h"
#include "dialog/runscriptdialog.h"
#include "formatsupport.h"
#include "job/imagejobcontext.h"
#include "network/completionhandlers.h"
#include "network/networkactions.h"
#include "recentitems.h"

namespace geometrize
{

namespace common
{

namespace ui
{

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
    // TODO save svg
    return QFileDialog::getSaveFileName(parent, QWidget::tr("Save Image"), "", QWidget::tr("JPEG Image (*.jpg *.jpeg);;PNG Image(*.png);;BMP Image(*.bmp)"));
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

}

}

}
