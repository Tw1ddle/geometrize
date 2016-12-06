#ifndef IMAGEJOBCONTEXT_H
#define IMAGEJOBCONTEXT_H

#include <memory>

namespace geometrize
{

class BitmapData;
class ImageJobContextPrivate;

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
    std::unique_ptr<ImageJobContextPrivate> d;
};

}

#endif // IMAGEJOBCONTEXT_H
