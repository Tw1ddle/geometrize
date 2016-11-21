#include "imagejobcontext.h"

#include <QObject>

#include "lib/geometrizer/geometrizer/model.h"

#include "imagejobpreferences.h"

class ImageJobContextPrivate : public QObject
{
public:
    ImageJobContextPrivate()
    {
    }

    ImageJobContextPrivate& operator=(const ImageJobContextPrivate&) = delete;
    ImageJobContextPrivate(const ImageJobContextPrivate&) = delete;

private:
    bool m_paused; ///> Whether the job is running or paused.
    ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
};

ImageJobContext::ImageJobContext()
{

}
