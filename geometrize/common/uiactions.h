#pragma once

#include <QPair>
#include <QString>
#include <QUrl>

class QWidget;

namespace geometrize
{

namespace common
{

namespace ui
{

void openLaunchWindow();
bool isLaunchWindowOpen();
void bringLaunchWindowToFront();
void openAboutPage(QWidget* parent);
void openGlobalPreferences(QWidget* parent);
QString openImagePathPickerDialog(QWidget* parent);
QString openSaveImagePathPickerDialog(QWidget* parent);
QString openSaveSVGPathPickerDialog(QWidget* parent);
QString openSaveRasterizedSVGPathPickerDialog(QWidget* parent);
QString openSaveBitmapPathPickerDialog(QWidget* parent);
QString openSaveGeometryDataPathPickerDialog(QWidget* parent);
QString openSaveGIFPathPickerDialog(QWidget* parent);
QString openSaveCanvasAnimationPathPickerDialog(QWidget* parent);
QString openSaveWebGLPathPickerDialog(QWidget* parent);
QString openSaveRawImageDataPathPickerDialog(QWidget* parent);
QUrl openGetUrlDialog(QWidget* parent);
QString openGetScriptDialog(QWidget* parent);
QString openSelectScriptDialog(QWidget* parent);
void openTechnicalSupport();
void openOnlineTutorials();
void openTechnicalSupport();
void openMoreResourcesPage();
QString openLoadImageJobSettingsDialog(QWidget* parent);
QString openSaveImageJobSettingsDialog(QWidget* parent);
QString openLoadGlobalSettingsDialog(QWidget* parent);
QString openSaveGlobalSettingsDialog(QWidget* parent);
void openSVGPreviewPage(QWidget* parent);

}

}

}
