#pragma once

#include <memory>

#include <QString>

namespace geometrize
{

class BitmapData;

/**
 * @brief The ImageJobContext class contains the data worked on by an image job.
 */
class ImageJobContext
{
public:
    ImageJobContext(const QString& displayName,BitmapData& bitmap);
    ImageJobContext& operator=(const ImageJobContext&) = delete;
    ImageJobContext(const ImageJobContext&) = delete;
    ~ImageJobContext() = default;

    /**
     * @brief getDisplayName Gets the display name of the image job.
     * @return The display name of the image job.
     */
    QString getDisplayName() const;

private:    
    class ImageJobContextImpl;
    std::unique_ptr<ImageJobContextImpl> d;
};

}
