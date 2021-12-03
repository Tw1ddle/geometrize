#pragma once

#include <memory>
#include <optional>
#include <string>

class QImage;

namespace geometrize
{

namespace preferences
{

class ImageTaskPreferences;

}

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
 * @param prefs Initial image task preferences to (optionally) set for the task.
 * @return The created image task object.
 */
ImageTask* createImageTaskAndWindow(const std::string& displayName, const std::string& taskUrl, const std::optional<geometrize::preferences::ImageTaskPreferences>& prefs);

/**
 * @brief createImageTaskAndWindow Creates an image task, and immediately creates a graphical window for manipulating the task.
 * @param displayName The display name of the image task.
 * @param image The image that the task will work on.
 * @param prefs Initial image task preferences to (optionally) set for the task.
 * @return The created image task object.
 */
ImageTask* createImageTaskAndWindow(const std::string& displayName, const QImage& image, const std::optional<geometrize::preferences::ImageTaskPreferences>& prefs);

}

}
