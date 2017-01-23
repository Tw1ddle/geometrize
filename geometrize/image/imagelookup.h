#pragma once

#include <memory>
#include <string>

namespace geometrize
{
class BitmapData;
}

namespace geometrize
{

namespace image
{

/**
 * @brief The ImageLookup class provides a mechanism for storing and retrieving bitmap data objects.
 */
class ImageLookup
{
public:
    ImageLookup();
    ImageLookup& operator=(const ImageLookup&) = delete;
    ImageLookup(const ImageLookup&) = delete;
    ~ImageLookup();

    BitmapData* getImage(const std::string& id);
    void setImage(const std::string& id, BitmapData* bitmapData);

private:
    class ImageLookupImpl;
    std::unique_ptr<ImageLookupImpl> d;
};

}

}
