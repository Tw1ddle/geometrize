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

// TODO
/**
 * QImage imageForImagePath(const QString& imagePath)
{
    return QImage(imagePath);
}

bool isImageValid(const QImage& image)
{
    return !image.isNull() && image.width() > 0 && image.height() > 0;
}

bool isPixmapValid(const QPixmap& pixmap)
{
    return !pixmap.isNull() && pixmap.width() > 0 && pixmap.height() > 0;
}

QPixmap openPixmap(QWidget* parent, const QString& imagePath)
{
    const auto openPix = [&](QWidget* parent, const QString& imagePath) {
        QImage image(imagePath);
        if(!isImageValid(image)) {
            QMessageBox::critical(parent, QWidget::tr("Image Loading Error"), QWidget::tr("The image file could not be loaded."));
            return QPixmap();
        }

        image = image.convertToFormat(QImage::Format_RGBA8888);

        const QPixmap pixmap{QPixmap::fromImage(image)};
        if(!isPixmapValid(pixmap)) {
            QMessageBox::critical(parent, QWidget::tr("Image Loading Error"), QWidget::tr("Failed to convert image file."));
            return QPixmap();
        }

        return pixmap;

        // TODO open svg, need to specify size after the fact
        // TODO separate this logic from the error boxes and this code path with signals
        // TODO create a new image job context, add a new tab for the image, add the image background initially
    };

    const QPixmap pixmap{openPix(parent, imagePath)};
    return pixmap;
}
*/
