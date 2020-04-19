#include "imagetaskcreator.h"

#include <QImage>
#include <QSize>

#include "imagetask.h"
#include "dialog/imagetaskwindow.h"
#include "image/imageloader.h"
#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace task
{

ImageTask* createImageTaskAndWindow(const std::string& displayName, const std::string& taskUrl)
{
    geometrize::Bitmap bitmap{image::convertImageToBitmapWithDownscaling(geometrize::image::loadImage(taskUrl))};
    ImageTask* task{new ImageTask(displayName, bitmap)};
    dialog::ImageTaskWindow* imageTaskWindow{new dialog::ImageTaskWindow()};
    imageTaskWindow->setImageTask(task);
    imageTaskWindow->show();
    return task;
}

ImageTask* createImageTaskAndWindow(const std::string& displayName, const QImage& image)
{
    geometrize::Bitmap bitmap{image::convertImageToBitmapWithDownscaling(image)};
    ImageTask* task{new ImageTask(displayName, bitmap)};
    dialog::ImageTaskWindow* imageTaskWindow{new dialog::ImageTaskWindow()};
    imageTaskWindow->setImageTask(task);
    imageTaskWindow->show();
    return task;
}

}

}
