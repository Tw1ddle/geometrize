#include "image/imagelookup.h"

#include <map>

#include "geometrize/bitmap/bitmapdata.h"

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

    BitmapData* getImage(const std::string& id)
    {
        return m_images[id];
    }

    void setImage(const std::string& id, BitmapData* bitmapData)
    {
        m_images[id] = bitmapData;
    }

private:
    std::map<std::string, BitmapData*> m_images;
};

BitmapData* ImageLookup::getImage(const std::string& id)
{
    return d->getImage(id);
}

void ImageLookup::setImage(const std::string& id, BitmapData* bitmapData)
{
    d->setImage(id, bitmapData);
}

ImageLookup::ImageLookup() : d{std::make_unique<ImageLookup::ImageLookupImpl>()}
{

}

ImageLookup::~ImageLookup()
{

}

}

}
