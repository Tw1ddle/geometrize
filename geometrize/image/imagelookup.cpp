#include "image/imagelookup.h"

#include <map>

#include "geometrize/core.h"

namespace geometrize
{

namespace image
{

class ImageLookup::ImageLookupImpl
{
public:
    ImageLookupImpl() = default;
    ImageLookupImpl& operator=(const ImageLookupImpl&) = delete;
    ImageLookupImpl(const ImageLookupImpl&) = delete;
    ~ImageLookupImpl() = default;

    Bitmap* getImage(const std::string& id)
    {
        return m_images[id];
    }

    void setImage(const std::string& id, Bitmap* bitmap)
    {
        m_images[id] = bitmap;
    }

private:
    std::map<std::string, Bitmap*> m_images;
};

Bitmap* ImageLookup::getImage(const std::string& id)
{
    return d->getImage(id);
}

void ImageLookup::setImage(const std::string& id, Bitmap* bitmap)
{
    d->setImage(id, bitmap);
}

ImageLookup::ImageLookup() : d{std::make_unique<ImageLookup::ImageLookupImpl>()}
{

}

ImageLookup::~ImageLookup()
{

}

}

}
