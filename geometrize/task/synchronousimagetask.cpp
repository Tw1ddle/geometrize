#include "synchronousimagetask.h"

#include <algorithm>
#include <vector>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/commonutil.h"
#include "geometrize/shaperesult.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/model.h"
#include "geometrize/shape/shapemutator.h"

#include "preferences/imagetaskpreferences.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace task
{

class SynchronousImageTask::SynchronousImageTaskImpl
{
public:
    SynchronousImageTaskImpl(SynchronousImageTask* pQ, Bitmap& target) :
        q{pQ},
        m_task{std::make_unique<geometrize::task::ImageTask>(target, Qt::DirectConnection)},
        m_stepConnection{}
    {
        m_stepConnection = q->connect(m_task.get(), &ImageTask::signal_modelDidStep, [this](const std::vector<geometrize::ShapeResult> shapes) {
            std::copy(shapes.begin(), shapes.end(), std::back_inserter(m_shapes));
        });
    }
    SynchronousImageTaskImpl operator=(const SynchronousImageTaskImpl&) = delete;
    SynchronousImageTaskImpl(const SynchronousImageTaskImpl&) = delete;
    ~SynchronousImageTaskImpl()
    {
        QObject::disconnect(m_stepConnection);
    }

    Bitmap& getTarget()
    {
        return m_task->getTargetMutable();
    }

    Bitmap& getCurrent()
    {
        return m_task->getCurrentMutable();
    }

    void step()
    {
        m_task->stepModel();
    }

    void drawBackgroundRectangle()
    {
        const geometrize::rgba color{geometrize::commonutil::getAverageImageColor(m_task->getTarget())};
        m_task->drawBackgroundRectangle(color);
    }

    geometrize::preferences::ImageTaskPreferences& getPreferences()
    {
        return m_task->getPreferences();
    }

    void setPreferences(preferences::ImageTaskPreferences preferences)
    {
        m_task->setPreferences(preferences);
    }

    std::vector<geometrize::ShapeResult> getShapes() const
    {
        return m_shapes;
    }

private:
    SynchronousImageTask* q;
    std::vector<geometrize::ShapeResult> m_shapes;
    geometrize::preferences::ImageTaskPreferences m_preferences;
    std::unique_ptr<geometrize::task::ImageTask> m_task;
    QMetaObject::Connection m_stepConnection;
};

SynchronousImageTask::SynchronousImageTask(Bitmap& target) : QObject{}, d{std::make_unique<SynchronousImageTask::SynchronousImageTaskImpl>(this, target)}
{
}

SynchronousImageTask::~SynchronousImageTask()
{
}

Bitmap& SynchronousImageTask::getTarget()
{
    return d->getTarget();
}

Bitmap& SynchronousImageTask::getCurrent()
{
    return d->getCurrent();
}

void SynchronousImageTask::stepModel()
{
    d->step();
}

void SynchronousImageTask::drawBackgroundRectangle()
{
    d->drawBackgroundRectangle();
}

geometrize::preferences::ImageTaskPreferences& SynchronousImageTask::getPreferences()
{
    return d->getPreferences();
}

void SynchronousImageTask::setPreferences(const preferences::ImageTaskPreferences preferences)
{
    d->setPreferences(preferences);
}

std::vector<geometrize::ShapeResult> SynchronousImageTask::getShapes() const
{
    return d->getShapes();
}

}

}
