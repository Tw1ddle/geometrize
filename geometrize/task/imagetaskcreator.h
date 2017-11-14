#pragma once

#include <memory>
#include <string>

class QImage;

namespace geometrize
{

namespace task
{

class ImageTask;

}

}

namespace geometrize
{

namespace task
{

/**
 * @brief createImageTaskAndWindow Creates an image task, and immediately creates a graphical window for manipulating the task.
 * @param displayName The display name of the image task.
 * @param taskUrl The URL of the task data source.
 * @return The created image task object.
 */
ImageTask* createImageTaskAndWindow(const std::string& displayName, const std::string& taskUrl);

/**
 * @brief createImageTaskAndWindow Creates an image task, and immediately creates a graphical window for manipulating the task.
 * @param displayName The display name of the image task.
 * @param image The image that the task will work on.
 * @return The created image task object.
 */
ImageTask* createImageTaskAndWindow(const std::string& displayName, const QImage& image);

}

}
