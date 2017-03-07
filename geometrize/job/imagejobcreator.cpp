#include "imagejobcreator.h"

#include <assert.h>

#include <QImage>

#include "geometrize/core.h"

#include "dialog/imagejobwindow.h"
#include "imagejob.h"
#include "common/sharedapp.h"
#include "joblookup.h"
#include "image/imageloader.h"

namespace geometrize
{

namespace job
{

ImageJob* createImageJob(const std::string& displayName, const std::string& jobUrl)
{
    const QImage image{image::loadImage(QString::fromStdString(jobUrl))};
    Bitmap bitmap(image::createBitmap(image));

    ImageJob* job{new ImageJob(displayName, jobUrl, bitmap)};
    common::app::SharedApp::get().getJobLookup().setImageJob(displayName, job);
    return job;
}

ImageJob* createImageJob(const std::string& displayName, const QImage& image)
{
    Bitmap bitmap(image::createBitmap(image));

    ImageJob* job{new ImageJob(displayName, "", bitmap)};
    common::app::SharedApp::get().getJobLookup().setImageJob(displayName, job);
    return job;
}

void createImageJobAndWindow(const std::string& displayName, const std::string& jobUrl)
{
    ImageJob* job{createImageJob(displayName, jobUrl)};

    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->setImageJob(job);
    imageJobWindow->show();
}

void createImageJobAndWindow(const std::string& displayName, const QImage& image)
{
    ImageJob* job{createImageJob(displayName, image)};

    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->setImageJob(job);
    imageJobWindow->show();
}

}

}
