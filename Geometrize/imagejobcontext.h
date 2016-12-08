#ifndef IMAGEJOBCONTEXT_H
#define IMAGEJOBCONTEXT_H

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

#endif // IMAGEJOBCONTEXT_H
