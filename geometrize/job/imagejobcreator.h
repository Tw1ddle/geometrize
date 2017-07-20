#pragma once

#include <memory>
#include <string>

class QImage;

namespace geometrize
{

namespace job
{

/**
 * @brief createImageJobAndWindow Creates an image job, and immediately creates a graphical window for manipulating the job.
 * @param displayName The display name of the image job.
 * @param jobUrl The URL of the job data source.
 */
void createImageJobAndWindow(const std::string& displayName, const std::string& jobUrl);

/**
 * @brief createImageJobAndWindow Creates an image job, and immediately creates a graphical window for manipulating the job.
 * @param displayName The display name of the image job.
 * @param image The image that the job will work on.
 */
void createImageJobAndWindow(const std::string& displayName, const QImage& image);

}

}
