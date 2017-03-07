#pragma once

#include <string>

class QImage;

namespace geometrize
{

namespace job
{
class ImageJob;
}

}

namespace geometrize
{

namespace job
{

/**
 * @brief createImageJob Creates an image job, adding it to the shared image job lookup.
 * @param displayName The display name of the image job.
 * @param jobUrl The URL of the job data source.
 * @return The new image job.
 */
ImageJob* createImageJob(const std::string& displayName, const std::string& jobUrl);

/**
 * @brief createImageJob Creates an image job, adding it to the shared image job lookup.
 * @param displayName The display name of the image job.
 * @param image The image that the job will work on.
 * @return The new image job.
 */
ImageJob* createImageJob(const std::string& displayName, const QImage& image);

/**
 * @brief createImageJobAndWindow Creates an image job, adding it to the shared image job lookup, and immediately creates a graphical window for manipulating the job.
 * @param displayName The display name of the image job.
 * @param jobUrl The URL of the job data source.
 */
void createImageJobAndWindow(const std::string& displayName, const std::string& jobUrl);

/**
 * @brief createImageJobAndWindow Creates an image job, adding it to the shared image job lookup, and immediately creates a graphical window for manipulating the job.
 * @param displayName The display name of the image job.
 * @param image The image that the job will work on.
 */
void createImageJobAndWindow(const std::string& displayName, const QImage& image);

}

}
