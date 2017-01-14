#include "imagejobcontext.h"

#include <QObject>

#include "preferences/imagejobpreferences.h"

#include "lib/geometrizer/geometrizer/bitmap/bitmapdata.h"
#include "lib/geometrizer/geometrizer/runner/imagerunner.h"

namespace geometrize
{

class ImageJobContext::ImageJobContextImpl : public QObject
{
public:
    ImageJobContextImpl(const QString& displayName, BitmapData& bitmap) : m_paused{true}, m_preferences{}, m_displayName{displayName}, m_runner{bitmap}
    {
    }

    ImageJobContextImpl& operator=(const ImageJobContextImpl&) = delete;
    ImageJobContextImpl(const ImageJobContextImpl&) = delete;

    QString getDisplayName() const
    {
        return m_displayName;
    }

private:
    bool m_paused; ///> Whether the job is running or paused.
    preferences::ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    QString m_displayName; ///> The display name of the image job.
    ImageRunner m_runner;
};

ImageJobContext::ImageJobContext(const QString& displayName, BitmapData& bitmap) : d{std::make_unique<ImageJobContext::ImageJobContextImpl>(displayName, bitmap)}
{

}

QString ImageJobContext::getDisplayName() const
{
    return d->getDisplayName();
}

}
