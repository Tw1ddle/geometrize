#include "imagejob.h"

#include <atomic>
#include <vector>

#include <QDebug>
#include <QObject>
#include <QThread>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/shaperesult.h"

#include "imagejobworker.h"
#include "preferences/imagejobpreferences.h"

namespace geometrize
{

namespace job
{

class ImageJob::ImageJobImpl
{
public:
    ImageJobImpl(ImageJob* pQ, const std::string& displayName, const std::string& jobUrl, Bitmap& bitmap) :
        q{pQ}, m_preferences{}, m_displayName{displayName}, m_jobUrl{jobUrl}, m_id{getId()}, m_worker{bitmap}
    {
        init();
    }

    ImageJobImpl(ImageJob* pQ, const std::string& displayName, const std::string& jobUrl, Bitmap& bitmap, const Bitmap& initial) :
        q{pQ}, m_preferences{}, m_displayName{displayName}, m_jobUrl{jobUrl}, m_id{getId()}, m_worker{bitmap, initial}
    {
        init();
    }

    ~ImageJobImpl()
    {
        m_workerThread.quit();
        if(!m_workerThread.wait(1000)) {
            m_workerThread.terminate();
            m_workerThread.wait();
        }
    }

    ImageJobImpl& operator=(const ImageJobImpl&) = delete;
    ImageJobImpl(const ImageJobImpl&) = delete;

    Bitmap& getTarget()
    {
        return m_worker.getTarget();
    }

    Bitmap& getCurrent()
    {
        return m_worker.getCurrent();
    }

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

    void stepModel()
    {
        emit q->signal_step();
    }

    void modelWillStep()
    {
        emit q->signal_modelWillStep();
    }

    void modelDidStep(std::vector<geometrize::ShapeResult> shapes)
    {
        emit q->signal_modelDidStep(shapes);
    }

    void applyPreferences(const preferences::ImageJobPreferences& preferences)
    {
        // TODO
    }

    preferences::ImageJobPreferences& getPreferences()
    {
        return m_preferences;
    }

private:
    static int getId()
    {
        static std::atomic_int id = 0;
        return id++;
    }

    void init()
    {
        qRegisterMetaType<std::vector<geometrize::ShapeResult>>();

        m_worker.moveToThread(&m_workerThread);
        m_workerThread.start();

        q->connect(q, &ImageJob::signal_step, &m_worker, &ImageJobWorker::step);
        q->connect(&m_worker, &ImageJobWorker::signal_willStep, q, &ImageJob::signal_modelWillStep);
        q->connect(&m_worker, &ImageJobWorker::signal_didStep, q, &ImageJob::signal_modelDidStep);
    }

    ImageJob* q;
    preferences::ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    std::string m_displayName; ///> The display name of the image job.
    std::string m_jobUrl; ///> The URL/original source of the data for the image job.
    const int m_id; ///> A unique id for the image job.
    QThread m_workerThread; ///> Thread that the image job worker runs on.
    ImageJobWorker m_worker; ///> The image job worker.
};

ImageJob::ImageJob(const std::string& displayName, const std::string& jobUrl, Bitmap& bitmap) :  QObject(),
    d{std::make_unique<ImageJob::ImageJobImpl>(this, displayName, jobUrl, bitmap)}
{
}

ImageJob::ImageJob(const std::string& displayName, const std::string& jobUrl, Bitmap& bitmap, const Bitmap& initial) : QObject(),
    d{std::make_unique<ImageJob::ImageJobImpl>(this, displayName, jobUrl, bitmap, initial)}
{
}

Bitmap& ImageJob::getTarget()
{
    return d->getTarget();
}

Bitmap& ImageJob::getCurrent()
{
    return d->getCurrent();
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

void ImageJob::stepModel()
{
    d->stepModel();
}

void ImageJob::modelWillStep()
{
    d->modelWillStep();
}

void ImageJob::modelDidStep(std::vector<geometrize::ShapeResult> shapes)
{
    d->modelDidStep(shapes);
}

void ImageJob::applyPreferences(const preferences::ImageJobPreferences& preferences)
{
    d->applyPreferences(preferences);
}

preferences::ImageJobPreferences& ImageJob::getPreferences()
{
    return d->getPreferences();
}

}

}
