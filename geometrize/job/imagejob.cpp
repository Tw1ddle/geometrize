#include "imagejob.h"

#include <QPixmap>
#include <QObject>

#include "preferences/imagejobpreferences.h"

#include "geometrize/bitmap/bitmapdata.h"
#include "geometrize/runner/imagerunner.h"

namespace geometrize
{

namespace job
{

class ImageJob::ImageJobImpl : public QObject
{
public:
    ImageJobImpl(const std::string& displayName, BitmapData& bitmap) : m_paused{true}, m_preferences{}, m_displayName{displayName}, m_runner{bitmap}
    {
    }

    ImageJobImpl& operator=(const ImageJobImpl&) = delete;
    ImageJobImpl(const ImageJobImpl&) = delete;

    std::string getDisplayName() const
    {
        return m_displayName;
    }

private:
    bool m_paused; ///> Whether the job is running or paused.
    preferences::ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    std::string m_displayName; ///> The display name of the image job.
    ImageRunner m_runner;
};

ImageJob::ImageJob(const std::string& displayName, BitmapData& bitmap) : d{std::make_unique<ImageJob::ImageJobImpl>(displayName, bitmap)}
{

}

std::string ImageJob::getDisplayName() const
{
    return d->getDisplayName();
}

}

}
