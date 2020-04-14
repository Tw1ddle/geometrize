#pragma once

#include <vector>

#include <QPair>
#include <QString>
#include <QUrl>

class QScreen;
class QWidget;

namespace geometrize
{

namespace common
{

namespace ui
{

const QScreen* getFirstScreenContainingCursor();
void centerWidgetOnScreen(QWidget& widget, const QScreen& screen);

void openLaunchWindow();
bool isLaunchWindowOpen();
void bringLaunchWindowToFront();
void openTaskQueueWindow();
void openWelcomePage();
void openAboutPage(QWidget* parent);
void openGlobalPreferences(QWidget* parent);
QString openGenericFolderPickerDialog(QWidget* parent);
QString openImagePathPickerDialog(QWidget* parent);
QString openSaveImagePathPickerDialog(QWidget* parent);
QString openSaveSVGPathPickerDialog(QWidget* parent);
QString openSaveRasterizedSVGPathPickerDialog(QWidget* parent);
QString openSaveRasterizedSVGsPathPickerDialog(QWidget* parent);
QString openSaveGeometryDataPathPickerDialog(QWidget* parent);
QString openSaveGIFPathPickerDialog(QWidget* parent);
QString openSaveCanvasPathPickerDialog(QWidget* parent);
QString openSaveWebGLPathPickerDialog(QWidget* parent);
QUrl openGetUrlDialog(QWidget* parent);
QString openGetScriptDialog(QWidget* parent);
QString openSelectScriptDialog(QWidget* parent);
void openTechnicalSupport();
void openOnlineTutorials();
void openTechnicalSupport();
void openMoreResourcesPage();
QString openLoadImageTaskSettingsDialog(QWidget* parent);
QString openSaveImageTaskSettingsDialog(QWidget* parent);
QString openLoadGlobalSettingsDialog(QWidget* parent);
QString openSaveGlobalSettingsDialog(QWidget* parent);
QString openTargetImagePickerDialog(QWidget* parent);

}

}

}
