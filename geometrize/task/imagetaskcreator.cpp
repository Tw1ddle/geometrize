#include "imagetaskcreator.h"

#include <optional>

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QSize>

#include "imagetask.h"
#include "dialog/imagetaskwindow.h"
#include "image/imageloader.h"
#include "preferences/globalpreferences.h"

namespace
{

// Replace alpha values in an image with a preferred value from global preferences (if the preference is enabled)
QImage replaceAlphaInImage(const QImage& image)
{
    const geometrize::preferences::GlobalPreferences& prefs = geometrize::preferences::getGlobalPreferences();
    if(!prefs.shouldUseImageAlphaReplacementColor()) {
        return image;
    }

    const auto col = prefs.getTargetImageAlphaReplacementColor();
    const QColor color(col[0], col[1], col[2], col[3]);

    QImage im(image.size(), image.format());
    im.fill(color);
    im.setAlphaChannel(im);

    QPixmap combined(im.size());
    QPainter p(&combined);
    p.drawImage(QPoint(0, 0), im);
    p.drawImage(QPoint(0, 0), image);
    p.end();

    return combined.toImage();
}

}

namespace geometrize
{

namespace task
{

ImageTask* createImageTaskAndWindow(const std::string& displayName, const std::string& taskUrl, const std::optional<geometrize::preferences::ImageTaskPreferences>& prefs)
{
    geometrize::Bitmap bitmap{image::convertImageToBitmapWithDownscaling(replaceAlphaInImage(geometrize::image::loadImage(taskUrl)))};
    ImageTask* task{new ImageTask(displayName, bitmap)};
    if(prefs) {
        task->setPreferences(*prefs);
    }
    dialog::ImageTaskWindow* imageTaskWindow{new dialog::ImageTaskWindow()};
    imageTaskWindow->setImageTask(task);
    imageTaskWindow->show();
    return task;
}

ImageTask* createImageTaskAndWindow(const std::string& displayName, const QImage& image, const std::optional<geometrize::preferences::ImageTaskPreferences>& prefs)
{
    geometrize::Bitmap bitmap{image::convertImageToBitmapWithDownscaling(replaceAlphaInImage(image))};
    ImageTask* task{new ImageTask(displayName, bitmap)};
    if(prefs) {
        task->setPreferences(*prefs);
    }
    dialog::ImageTaskWindow* imageTaskWindow{new dialog::ImageTaskWindow()};
    imageTaskWindow->setImageTask(task);
    imageTaskWindow->show();
    return task;
}

}

}
