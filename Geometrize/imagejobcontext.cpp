#include "imagejobcontext.h"

#include <QObject>

#include "imagejobpreferences.h"

#include "lib/geometrizer/geometrizer/bitmap/bitmapdata.h"
#include "lib/geometrizer/geometrizer/runner/imagerunner.h"

namespace geometrize
{

class ImageJobContextPrivate : public QObject
{
public:
    ImageJobContextPrivate(BitmapData& bitmap) : m_paused{true}, m_preferences{}, m_runner{bitmap}
    {
    }

    ImageJobContextPrivate& operator=(const ImageJobContextPrivate&) = delete;
    ImageJobContextPrivate(const ImageJobContextPrivate&) = delete;

private:
    bool m_paused; ///> Whether the job is running or paused.
    ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    ImageRunner m_runner;
};

ImageJobContext::ImageJobContext(BitmapData& bitmap) : d{std::make_unique<ImageJobContextPrivate>(bitmap)}
{

}

}
