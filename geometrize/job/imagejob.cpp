#include "imagejob.h"

#include <atomic>
#include <cassert>
#include <vector>

#include <QThread>

#include "geometrize/commonutil.h"
#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/model.h"
#include "geometrize/shaperesult.h"
#include "geometrize/shape/rectangle.h"

#include "job/imagejobworker.h"
#include "preferences/imagejobpreferences.h"
#include "script/shapemutationrules.h"

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

    chaiscript::ChaiScript* getEngine()
    {
        return m_mutationRules.getEngine();
    }

    Bitmap& getTarget()
    {
        return m_worker.getTarget();
    }

    Bitmap& getCurrent()
    {
        return m_worker.getCurrent();
    }

    const Bitmap& getTarget() const
    {
        return m_worker.getTarget();
    }

    const Bitmap& getCurrent() const
    {
        return m_worker.getCurrent();
    }

    std::uint32_t getWidth() const
    {
        return m_worker.getCurrent().getWidth();
    }

    std::uint32_t getHeight() const
    {
        return m_worker.getCurrent().getHeight();
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

    bool isStepping() const
    {
        return m_worker.isStepping();
    }

    void stepModel()
    {
        emit q->signal_step(m_preferences.getImageRunnerOptions());
    }

    void drawShape(std::shared_ptr<geometrize::Shape> shape, const geometrize::rgba color)
    {
        emit q->signal_drawShape(shape, color);
    }

    void drawBackgroundRectangle()
    {
        const geometrize::rgba color{geometrize::commonutil::getAverageImageColor(m_worker.getRunner().getTarget())};
        const std::shared_ptr<geometrize::Rectangle> rectangle = std::make_shared<geometrize::Rectangle>(m_worker.getRunner().getModel());
        rectangle->m_x1 = 0;
        rectangle->m_x2 = m_worker.getTarget().getWidth();
        rectangle->m_y1 = 0;
        rectangle->m_y2 = m_worker.getTarget().getHeight();
        emit q->signal_drawShape(rectangle, color);
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
        static std::atomic<std::size_t> id{0U};
        return id++;
    }

    void init()
    {
        qRegisterMetaType<std::vector<geometrize::ShapeResult>>();
        qRegisterMetaType<geometrize::ImageRunnerOptions>();
        qRegisterMetaType<std::shared_ptr<geometrize::Shape>>();
        qRegisterMetaType<geometrize::rgba>();

        m_worker.moveToThread(&m_workerThread);
        m_workerThread.start();

        connectSignals(Qt::AutoConnection);
    }

    void connectSignals(const Qt::ConnectionType connectionType)
    {
        q->connect(q, &ImageJob::signal_step, &m_worker, &ImageJobWorker::step, connectionType);
        q->connect(q, &ImageJob::signal_drawShape, &m_worker, &ImageJobWorker::drawShape, connectionType);
        q->connect(&m_worker, &ImageJobWorker::signal_willStep, q, &ImageJob::signal_modelWillStep, connectionType);
        q->connect(&m_worker, &ImageJobWorker::signal_didStep, q, &ImageJob::signal_modelDidStep, connectionType);
    }

    void disconnectAll()
    {
        q->disconnect(q, &ImageJob::signal_step, &m_worker, &ImageJobWorker::step);
        q->disconnect(q, &ImageJob::signal_drawShape, &m_worker, &ImageJobWorker::drawShape);
        q->disconnect(&m_worker, &ImageJobWorker::signal_willStep, q, &ImageJob::signal_modelWillStep);
        q->disconnect(&m_worker, &ImageJobWorker::signal_didStep, q, &ImageJob::signal_modelDidStep);
    }

    ImageJob* q;
    preferences::ImageJobPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    geometrize::ShapeMutationRules m_mutationRules; ///> The shape mutation rules for the image job.
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

ImageJob::~ImageJob()
{
}

chaiscript::ChaiScript* ImageJob::getEngine()
{
    return d->getEngine();
}

Bitmap& ImageJob::getTarget()
{
    return d->getTarget();
}

Bitmap& ImageJob::getCurrent()
{
    return d->getCurrent();
}

const Bitmap& ImageJob::getTarget() const
{
    return d->getTarget();
}

const Bitmap& ImageJob::getCurrent() const
{
    return d->getCurrent();
}

std::uint32_t ImageJob::getWidth() const
{
    return d->getWidth();
}

std::uint32_t ImageJob::getHeight() const
{
    return d->getHeight();
}

std::string ImageJob::getDisplayName() const
{
    return d->getDisplayName();
}

std::size_t ImageJob::getJobId() const
{
    return d->getJobId();
}

bool ImageJob::isStepping() const
{
    return d->isStepping();
}

void ImageJob::stepModel()
{
    d->stepModel();
}

void ImageJob::drawShape(std::shared_ptr<geometrize::Shape> shape, const geometrize::rgba color)
{
    d->drawShape(shape, color);
}

void ImageJob::drawBackgroundRectangle()
{
    d->drawBackgroundRectangle();
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
