#pragma once

#include <memory>

#include <QString>

class QPixmap;

namespace geometrize
{
class BitmapData;
}

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
 * @brief The ImageJob class contains the data worked on by an image job.
 */
class ImageJob
{
public:
    ImageJob(const std::string& displayName, BitmapData& bitmap);
    ImageJob& operator=(const ImageJob&) = delete;
    ImageJob(const ImageJob&) = delete;
    ~ImageJob() = default;

    /**
     * @brief getDisplayName Gets the display name of the image job.
     * @return The display name of the image job.
     */
    std::string getDisplayName() const;

private:    
    class ImageJobImpl;
    std::unique_ptr<ImageJobImpl> d;
};

}

}
