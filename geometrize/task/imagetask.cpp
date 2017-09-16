#include "imagetask.h"

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

#include "preferences/imagetaskpreferences.h"
#include "script/shapemutationrules.h"
#include "task/imagetaskworker.h"

namespace geometrize
{

namespace task
{

class ImageTask::ImageTaskImpl
{
public:
    ImageTaskImpl(ImageTask* pQ, const std::string& displayName, Bitmap& bitmap) :
        q{pQ}, m_preferences{}, m_displayName{displayName}, m_id{getId()}, m_worker{bitmap}
    {
        init();
    }

    ImageTaskImpl(ImageTask* pQ, const std::string& displayName, Bitmap& bitmap, const Bitmap& initial) :
        q{pQ}, m_preferences{}, m_displayName{displayName}, m_id{getId()}, m_worker{bitmap, initial}
    {
        init();
    }

    ~ImageTaskImpl()
    {
        disconnectAll();

        m_workerThread.quit();
        if(!m_workerThread.wait(1000)) {
            m_workerThread.terminate();
            m_workerThread.wait();
        }
    }

    ImageTaskImpl& operator=(const ImageTaskImpl&) = delete;
    ImageTaskImpl(const ImageTaskImpl&) = delete;

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

    std::size_t getTaskId() const
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

    preferences::ImageTaskPreferences& getPreferences()
    {
        return m_preferences;
    }

    void setPreferences(const preferences::ImageTaskPreferences preferences)
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
        q->connect(q, &ImageTask::signal_step, &m_worker, &ImageTaskWorker::step, connectionType);
        q->connect(q, &ImageTask::signal_drawShape, &m_worker, &ImageTaskWorker::drawShape, connectionType);
        q->connect(&m_worker, &ImageTaskWorker::signal_willStep, q, &ImageTask::signal_modelWillStep, connectionType);
        q->connect(&m_worker, &ImageTaskWorker::signal_didStep, q, &ImageTask::signal_modelDidStep, connectionType);
    }

    void disconnectAll()
    {
        q->disconnect(q, &ImageTask::signal_step, &m_worker, &ImageTaskWorker::step);
        q->disconnect(q, &ImageTask::signal_drawShape, &m_worker, &ImageTaskWorker::drawShape);
        q->disconnect(&m_worker, &ImageTaskWorker::signal_willStep, q, &ImageTask::signal_modelWillStep);
        q->disconnect(&m_worker, &ImageTaskWorker::signal_didStep, q, &ImageTask::signal_modelDidStep);
    }

    ImageTask* q;
    preferences::ImageTaskPreferences m_preferences; ///> Runtime configuration parameters for the runner.
    geometrize::ShapeMutationRules m_mutationRules; ///> The shape mutation rules for the image task.
    const std::string m_displayName; ///> The display name of the image task.
    const std::size_t m_id; ///> A unique id for the image task.
    QThread m_workerThread; ///> Thread that the image task worker runs on.
    ImageTaskWorker m_worker; ///> The image task worker.
};

ImageTask::ImageTask(Bitmap& target) : QObject(),
    d{std::make_unique<ImageTask::ImageTaskImpl>(this, "", target)}
{
}

ImageTask::ImageTask(Bitmap& target, Bitmap& background) : QObject(),
    d{std::make_unique<ImageTask::ImageTaskImpl>(this, "", target, background)}
{
}

ImageTask::ImageTask(const std::string& displayName, Bitmap& bitmap) : QObject(),
    d{std::make_unique<ImageTask::ImageTaskImpl>(this, displayName, bitmap)}
{
}

ImageTask::ImageTask(const std::string& displayName, Bitmap& bitmap, const Bitmap& initial) : QObject(),
    d{std::make_unique<ImageTask::ImageTaskImpl>(this, displayName, bitmap, initial)}
{
}

ImageTask::~ImageTask()
{
}

chaiscript::ChaiScript* ImageTask::getEngine()
{
    return d->getEngine();
}

Bitmap& ImageTask::getTarget()
{
    return d->getTarget();
}

Bitmap& ImageTask::getCurrent()
{
    return d->getCurrent();
}

const Bitmap& ImageTask::getTarget() const
{
    return d->getTarget();
}

const Bitmap& ImageTask::getCurrent() const
{
    return d->getCurrent();
}

std::uint32_t ImageTask::getWidth() const
{
    return d->getWidth();
}

std::uint32_t ImageTask::getHeight() const
{
    return d->getHeight();
}

std::string ImageTask::getDisplayName() const
{
    return d->getDisplayName();
}

std::size_t ImageTask::getTaskId() const
{
    return d->getTaskId();
}

bool ImageTask::isStepping() const
{
    return d->isStepping();
}

void ImageTask::stepModel()
{
    d->stepModel();
}

void ImageTask::drawShape(std::shared_ptr<geometrize::Shape> shape, const geometrize::rgba color)
{
    d->drawShape(shape, color);
}

void ImageTask::drawBackgroundRectangle()
{
    d->drawBackgroundRectangle();
}

void ImageTask::modelWillStep()
{
    d->modelWillStep();
}

void ImageTask::modelDidStep(std::vector<geometrize::ShapeResult> shapes)
{
    d->modelDidStep(shapes);
}

preferences::ImageTaskPreferences& ImageTask::getPreferences()
{
    return d->getPreferences();
}

void ImageTask::setPreferences(const preferences::ImageTaskPreferences preferences)
{
    d->setPreferences(preferences);
}

}

}
