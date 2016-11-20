#include "imagejobcontext.h"

#include <QObject>

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
};

ImageJobContext::ImageJobContext()
{

}
