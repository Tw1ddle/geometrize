#include "imagejobcreator.h"

#include "geometrize/bitmap/bitmapdata.h"
#include "geometrize/bitmap/rgba.h"

#include "dialog/imagejobwindow.h"
#include "imagejob.h"

namespace geometrize
{

namespace job
{

void createImageJobAndWindow(const std::string& displayName)
{
    dialog::ImageJobWindow* imageJobWindow{new dialog::ImageJobWindow()};
    imageJobWindow->show();

    new ImageJob(displayName, BitmapData(10, 10, rgba{0, 0, 0, 0})); // TODO
}

}

}
