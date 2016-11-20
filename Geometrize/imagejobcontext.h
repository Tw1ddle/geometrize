#ifndef IMAGEJOBCONTEXT_H
#define IMAGEJOBCONTEXT_H

#include <memory>

class ImageJobContextPrivate;

class ImageJobContext
{
public:
    ImageJobContext();
    ImageJobContext& operator=(const ImageJobContext&) = delete;
    ImageJobContext(const ImageJobContext&) = delete;

private:
    std::unique_ptr<ImageJobContextPrivate> d;
};

#endif // IMAGEJOBCONTEXT_H
