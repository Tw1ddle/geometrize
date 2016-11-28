#ifndef IMAGEJOBCONTEXT_H
#define IMAGEJOBCONTEXT_H

#include <memory>

namespace geometrize
{

class ImageJobContextPrivate;
class BitmapData;

class ImageJobContext
{
public:
    ImageJobContext(BitmapData& bitmap);
    ImageJobContext& operator=(const ImageJobContext&) = delete;
    ImageJobContext(const ImageJobContext&) = delete;

private:
    std::unique_ptr<ImageJobContextPrivate> d;
};

}

#endif // IMAGEJOBCONTEXT_H
