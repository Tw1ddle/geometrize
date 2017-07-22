#include "imagejob.h"

#include <atomic>
#include <cassert>
#include <vector>

#include <QThread>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/model.h"
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
    ImageJobImpl(ImageJob* pQ, const std::string& displayName, Bitmap& bitmap) :
        q{pQ}, m_preferences{}, m_displayName{displayName}, m_id{getId()}, m_worker{bitmap}
    {
        init();
    }

    ImageJobImpl(ImageJob* pQ, const std::string& displayName, Bitmap& bitmap, const Bitmap& initial) :
        q{pQ}, m_preferences{}, m_displayName{displayName}, m_id{getId()}, m_worker{bitmap, initial}
    {
        init();
    }

    ~ImageJobImpl()
    {
        disconnectAll();

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

    ShapeMutator& getShapeMutator()
    {
        return m_worker.getRunner().getModel().getShapeMutator();
    }

    std::string getDisplayName() const
    {
        return m_displayName;
    }

    std::size_t getJobId() const
    {
        return m_id;
    }

    void stepModel()
    {
        emit q->signal_step(m_preferences.getImageRunnerOptions());
    }

    void modelWillStep()
    {
        emit q->signal_modelWillStep();
    }

    void modelDidStep(std::vector<geometrize::ShapeResult> shapes)
    {
        emit q->signal_modelDidStep(shapes);
    }

    preferences::ImageJobPreferences& getPreferences()
    {
        return m_preferences;
    }

    void setPreferences(const preferences::ImageJobPreferences preferences)
    {
        m_preferences = preferences;
    }

private:
    static std::size_t getId()
    {
        static std::atomic<std::size_t> id = 0;
        return id++;
    }

    void init()
    {
        qRegisterMetaType<std::vector<geometrize::ShapeResult>>();
        qRegisterMetaType<geometrize::ImageRunnerOptions>();

        m_worker.moveToThread(&m_workerThread);
        m_workerThread.start();

        // NOTE should use direct connections for synchronous/immediate return behavior when running scripts/CLI
        connectSignals(Qt::QueuedConnection);
    }

    void connectSignals(const Qt::ConnectionType connectionType)
    {
        q->connect(q, &ImageJob::signal_step, &m_worker, &ImageJobWorker::step, connectionType);
        q->connect(&m_worker, &ImageJobWorker::signal_willStep, q, &ImageJob::signal_modelWillStep, connectionType);
        q->connect(&m_worker, &ImageJobWorker::signal_didStep, q, &ImageJob::signal_modelDidStep, connectionType);
    }

    void disconnectAll()
    {
        q->disconnect(q, &ImageJob::signal_step, &m_worker, &ImageJobWorker::step);
        q->disconnect(&m_worker, &ImageJobWorker::signal_willStep, q, &ImageJob::signal_modelWillStep);
        q->disconnect(&m_worker, &ImageJobWorker::signal_didStep, q, &ImageJob::signal_modelDidStep);
    }

    ImageJob* q;
    preferences::ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    const std::string m_displayName; ///> The display name of the image job.
    const std::size_t m_id; ///> A unique id for the image job.
    QThread m_workerThread; ///> Thread that the image job worker runs on.
    ImageJobWorker m_worker; ///> The image job worker.
};

ImageJob::ImageJob(Bitmap& target) : QObject(),
    d{std::make_unique<ImageJob::ImageJobImpl>(this, "", target)}
{
}

ImageJob::ImageJob(Bitmap& target, Bitmap& background) : QObject(),
    d{std::make_unique<ImageJob::ImageJobImpl>(this, "", target, background)}
{
}

ImageJob::ImageJob(const std::string& displayName, Bitmap& bitmap) : QObject(),
    d{std::make_unique<ImageJob::ImageJobImpl>(this, displayName, bitmap)}
{
}

ImageJob::ImageJob(const std::string& displayName, Bitmap& bitmap, const Bitmap& initial) : QObject(),
    d{std::make_unique<ImageJob::ImageJobImpl>(this, displayName, bitmap, initial)}
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

ShapeMutator& ImageJob::getShapeMutator()
{
    return d->getShapeMutator();
}

std::string ImageJob::getDisplayName() const
{
    return d->getDisplayName();
}

std::size_t ImageJob::getJobId() const
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

preferences::ImageJobPreferences& ImageJob::getPreferences()
{
    return d->getPreferences();
}

void ImageJob::setPreferences(const preferences::ImageJobPreferences preferences)
{
    d->setPreferences(preferences);
}

}

}
