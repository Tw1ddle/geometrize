#include "imagejob.h"

#include <atomic>
#include <vector>

#include <QPixmap>
#include <QObject>

#include "geometrize/runner/imagerunner.h"
#include "geometrize/bitmap/bitmapdata.h"
#include "geometrize/shaperesult.h"

#include "preferences/imagejobpreferences.h"

namespace geometrize
{

namespace job
{

class ImageJob::ImageJobImpl : public QObject
{
public:
    ImageJobImpl(const std::string& displayName, const std::string& jobUrl, BitmapData& bitmap) : m_paused{true}, m_preferences{}, m_displayName{displayName}, m_jobUrl{jobUrl}, m_id{getId()}, m_runner{bitmap}
    {
    }

    ImageJobImpl& operator=(const ImageJobImpl&) = delete;
    ImageJobImpl(const ImageJobImpl&) = delete;

    std::string getDisplayName() const
    {
        return m_displayName;
    }

    std::string getJobUrl() const
    {
        return m_jobUrl;
    }

    int getJobId() const
    {
        return m_id;
    }

    BitmapData& getBitmapData()
    {
        return m_runner.getBitmapData();
    }

    std::vector<geometrize::ShapeResult> stepModel()
    {
        return m_runner.step();
    }

private:
    static int getId()
    {
        static std::atomic_int id = 0;
        return id++;
    }

    bool m_paused; ///> Whether the job is running or paused.
    preferences::ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    std::string m_displayName; ///> The display name of the image job.
    std::string m_jobUrl; ///> The URL/original source of the data for the image job.
    const int m_id; ///> A unique id for the image job.
    ImageRunner m_runner;
};

ImageJob::ImageJob(const std::string& displayName, const std::string& jobUrl, BitmapData& bitmap) : d{std::make_unique<ImageJob::ImageJobImpl>(displayName, jobUrl, bitmap)}
{

}

std::string ImageJob::getDisplayName() const
{
    return d->getDisplayName();
}

std::string ImageJob::getJobUrl() const
{
    return d->getJobUrl();
}

int ImageJob::getJobId() const
{
    return d->getJobId();
}

BitmapData& ImageJob::getBitmapData()
{
    return d->getBitmapData();
}

std::vector<geometrize::ShapeResult> ImageJob::stepModel()
{
    emit signal_modelWillStep();
    const std::vector<ShapeResult> shapes{d->stepModel()};
    emit signal_modelDidStep(shapes);

    return shapes;
}

}

}
