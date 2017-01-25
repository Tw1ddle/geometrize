#pragma once

#include <QPair>
#include <QString>
#include <QUrl>

#include "script/scriptoptions.h"

class QWidget;

namespace geometrize
{

namespace job
{
class ImageJobContext;
}

}

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
void openPreferences(QWidget* parent);
int openQuitDialog(QWidget* parent);
QString openImagePathPickerDialog(QWidget* parent);
QString openSaveImagePathPickerDialog(QWidget* parent);
QUrl openGetUrlDialog(QWidget* parent);
QPair<QString, geometrize::script::ScriptOptions> openGetScriptDialog(QWidget* parent);
QString openSelectScriptDialog(QWidget* parent);
void openTechnicalSupport();
void openOnlineTutorials();
void openTechnicalSupport();
void openMoreResourcesPage();

}

}

}
