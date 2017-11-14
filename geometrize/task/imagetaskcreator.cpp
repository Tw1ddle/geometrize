#include "imagetaskcreator.h"

#include <QImage>
#include <QSize>

#include "imagetask.h"
#include "dialog/imagetaskwindow.h"
#include "image/imageloader.h"
#include "preferences/globalpreferences.h"

namespace
{

geometrize::Bitmap imageToBitmap(const QImage& image)
{
    QImage im{image.copy()};

    const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
    if(prefs.isImageTaskImageResizeEnabled()) {
        const std::pair<std::uint32_t, std::uint32_t> sizeThreshold{prefs.getImageTaskResizeThreshold()};
        const QSize imageSize{im.size()};

        if(sizeThreshold.first < static_cast<unsigned int>(imageSize.width())
                || sizeThreshold.second < static_cast<unsigned int>(imageSize.height())) {
            im = image.scaled(sizeThreshold.first, sizeThreshold.second, Qt::KeepAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGBA8888);
        }
    }

    return geometrize::image::createBitmap(im);
}

}

namespace geometrize
{

namespace task
{

ImageTask* createImageTaskAndWindow(const std::string& displayName, const std::string& taskUrl)
{
    geometrize::Bitmap bitmap{imageToBitmap(geometrize::image::loadImage(taskUrl))};
    ImageTask* task{new ImageTask(displayName, bitmap)};
    dialog::ImageTaskWindow* imageTaskWindow{new dialog::ImageTaskWindow()};
    imageTaskWindow->setImageTask(task);
    imageTaskWindow->show();
    return task;
}

ImageTask* createImageTaskAndWindow(const std::string& displayName, const QImage& image)
{
    geometrize::Bitmap bitmap{imageToBitmap(image)};
    ImageTask* task{new ImageTask(displayName, bitmap)};
    dialog::ImageTaskWindow* imageTaskWindow{new dialog::ImageTaskWindow()};
    imageTaskWindow->setImageTask(task);
    imageTaskWindow->show();
    return task;
}

}

}
