#include "imagetaskworker.h"

#include "geometrize/core.h"
#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/model.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace task
{

ImageTaskWorker::ImageTaskWorker(Bitmap& bitmap) : QObject(), m_runner{bitmap}, m_working{false}
{
}

ImageTaskWorker::ImageTaskWorker(Bitmap& bitmap, const Bitmap& initial) : QObject(), m_runner{bitmap, initial}, m_working{false}
{
}

ImageTaskWorker::~ImageTaskWorker()
{
}

void ImageTaskWorker::step(const geometrize::ImageRunnerOptions options,
                           const std::function<std::shared_ptr<geometrize::Shape>()> shapeCreator,
                           const geometrize::core::EnergyFunction energyFunction,
                           const geometrize::ShapeAcceptancePreconditionFunction addShapePreconditionFunction)
{
    emit signal_willStep();
    m_working = true;
    const std::vector<geometrize::ShapeResult> results{m_runner.step(options, shapeCreator, energyFunction, addShapePreconditionFunction)};
    m_working = false;
    emit signal_didStep(results);
}

void ImageTaskWorker::drawShape(const std::shared_ptr<geometrize::Shape> shape, const geometrize::rgba color)
{
    emit signal_willStep();
    m_working = true;
    const geometrize::ShapeResult result{m_runner.getModel().drawShape(shape, color)};
    m_working = false;
    emit signal_didStep({ result });
}

geometrize::Bitmap& ImageTaskWorker::getCurrent()
{
    return m_runner.getCurrent();
}

geometrize::Bitmap& ImageTaskWorker::getTarget()
{
    return m_runner.getTarget();
}

const geometrize::Bitmap& ImageTaskWorker::getCurrent() const
{
    return m_runner.getCurrent();
}

const geometrize::Bitmap& ImageTaskWorker::getTarget() const
{
    return m_runner.getTarget();
}

geometrize::ImageRunner& ImageTaskWorker::getRunner()
{
    return m_runner;
}

bool ImageTaskWorker::isStepping() const
{
    return m_working;
}

}

}
