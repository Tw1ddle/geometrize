#include "imagejobcontext.h"

#include <QObject>

#include "imagejobpreferences.h"

#include "lib/geometrizer/geometrizer/bitmap/bitmapdata.h"
#include "lib/geometrizer/geometrizer/runner/imagerunner.h"

namespace geometrize
{

class ImageJobContext::ImageJobContextImpl : public QObject
{
public:
    ImageJobContextImpl(BitmapData& bitmap) : m_paused{true}, m_preferences{}, m_runner{bitmap}
    {
    }

    ImageJobContextImpl& operator=(const ImageJobContextImpl&) = delete;
    ImageJobContextImpl(const ImageJobContextImpl&) = delete;

private:
    bool m_paused; ///> Whether the job is running or paused.
    ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    ImageRunner m_runner;
};

ImageJobContext::ImageJobContext(BitmapData& bitmap) : d{std::make_unique<ImageJobContext::ImageJobContextImpl>(bitmap)}
{

}

}
