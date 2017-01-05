#pragma once

#include <QPair>
#include <QPixmap>
#include <QString>
#include <QUrl>

#include "script/scriptoptions.h"

class QWidget;

namespace geometrize
{
class ImageJobContext;
}

namespace geometrize
{

namespace common
{

namespace ui
{

// Common UI actions
void openAboutPage(QWidget* parent);
void openPreferences(QWidget* parent);
int openQuitDialog(QWidget* parent);
QUrl openGetUrlDialog(QWidget* parent);
QPair<QString, geometrize::script::ScriptOptions> openGetScriptDialog(QWidget* parent);
QString openSelectScriptDialog(QWidget* parent);
void openTechnicalSupport();
void openOnlineTutorials();
void openTechnicalSupport();

// File picker
QPixmap openPixmap(QWidget* parent, const QString& imagePath);
void saveImage(QWidget* parent);
QString getImagePath(QWidget* parent);

// Image job creation/opening
void openJobs(const QStringList& urls);
ImageJobContext* createImageJob(QWidget* parent, const QString& displayName, const QPixmap& pixmap);
ImageJobContext* createImageJobAndUpdateRecents(QWidget* parent, const QUrl& url);

}

}

}
