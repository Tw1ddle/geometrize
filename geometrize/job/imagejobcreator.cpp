#include "imagejobcreator.h"

#include <assert.h>

#include <QImage>

#include "geometrize/core.h"

#include "dialog/imagejobwindow.h"
#include "imagejob.h"
#include "common/sharedapp.h"
#include "joblookup.h"
#include "image/imageloader.h"
#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace job
{

std::shared_ptr<ImageJob> createImageJob(const std::string& displayName, const QImage& image)
{
    QImage& im {image.copy()};

    const geometrize::preferences::GlobalPreferences& prefs{geometrize::common::app::SharedApp::get().getGlobalPreferences()};
    if(prefs.isImageJobImageResizeEnabled()) {
        const std::pair<std::uint32_t, std::uint32_t> sizeThreshold{prefs.getImageJobResizeThreshold()};
        const QSize imageSize{im.size()};

        if(sizeThreshold.first < static_cast<unsigned int>(imageSize.width())
                || sizeThreshold.second < static_cast<unsigned int>(imageSize.height())) {
            im = image.scaled(sizeThreshold.first, sizeThreshold.second, Qt::KeepAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGBA8888);
        }
    }

    Bitmap bitmap(image::createBitmap(im));

    std::shared_ptr<ImageJob> job{std::make_shared<ImageJob>(displayName, "", bitmap)};
    if(!job) {
        assert(0 && "Failed to create image job");
        return nullptr;
    }

    common::app::SharedApp::get().getJobLookup().setImageJob(displayName, job);
    return job;
}

std::shared_ptr<ImageJob> createImageJob(const std::string& displayName, const std::string& jobUrl)
{
    const QImage image{image::loadImage(QString::fromStdString(jobUrl))};
    if(image.isNull()) {
        assert(0 && "Failed to load image when creating image job");
        return nullptr;
    }
    return createImageJob(displayName, image);
}

void createImageJobAndWindow(const std::string& displayName, const std::string& jobUrl)
{
    std::shared_ptr<ImageJob> job{createImageJob(displayName, jobUrl)};
    if(!job) {
        return;
    }

    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->setImageJob(job);
    imageJobWindow->show();
}

void createImageJobAndWindow(const std::string& displayName, const QImage& image)
{
    std::shared_ptr<ImageJob> job{createImageJob(displayName, image)};
    if(!job) {
        return;
    }

    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->setImageJob(job);
    imageJobWindow->show();
}

}

}
