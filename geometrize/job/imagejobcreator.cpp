#include "imagejobcreator.h"

#include <QImage>
#include <QSize>

#include "dialog/imagejobwindow.h"
#include "imagejob.h"
#include "image/imageloader.h"
#include "preferences/globalpreferences.h"

namespace
{

geometrize::Bitmap imageToBitmap(const QImage& image)
{
    QImage im{image.copy()};

    const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
    if(prefs.isImageJobImageResizeEnabled()) {
        const std::pair<std::uint32_t, std::uint32_t> sizeThreshold{prefs.getImageJobResizeThreshold()};
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

namespace job
{

void createImageJobAndWindow(const std::string& displayName, const std::string& jobUrl)
{
    geometrize::Bitmap bitmap{imageToBitmap(geometrize::image::loadImage(jobUrl))};
    ImageJob* job{new ImageJob(displayName, bitmap)};
    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->setImageJob(job);
    imageJobWindow->show();
}

void createImageJobAndWindow(const std::string& displayName, const QImage& image)
{
    geometrize::Bitmap bitmap{imageToBitmap(image)};
    ImageJob* job{new ImageJob(displayName, bitmap)};
    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->setImageJob(job);
    imageJobWindow->show();
}

}

}
