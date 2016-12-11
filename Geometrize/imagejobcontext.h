#pragma once

#include <memory>

namespace geometrize
{

class BitmapData;

/**
 * @brief The ImageJobContext class contains the data worked on by an image job.
 */
class ImageJobContext
{
public:
    ImageJobContext(BitmapData& bitmap);
    ImageJobContext& operator=(const ImageJobContext&) = delete;
    ImageJobContext(const ImageJobContext&) = delete;
    ~ImageJobContext() = default;

private:
    class ImageJobContextImpl;
    std::unique_ptr<ImageJobContextImpl> d;
};

}
