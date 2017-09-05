#include "imagejobworker.h"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/model.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace job
{

ImageJobWorker::ImageJobWorker(Bitmap& bitmap) : QObject(), m_runner{bitmap}, m_working{false}
{
}

ImageJobWorker::ImageJobWorker(Bitmap& bitmap, const Bitmap& initial) : QObject(), m_runner{bitmap, initial}, m_working{false}
{
}

ImageJobWorker::~ImageJobWorker()
{
}

void ImageJobWorker::step(const geometrize::ImageRunnerOptions options)
{
    emit signal_willStep();
    m_working = true;
    const std::vector<geometrize::ShapeResult> results{m_runner.step(options)};
    m_working = false;
    emit signal_didStep(results);
}

void ImageJobWorker::drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color)
{
    emit signal_willStep();
    m_working = true;
    const geometrize::ShapeResult result{m_runner.getModel().drawShape(shape, color)};
    m_working = false;
    emit signal_didStep({ result });
}

geometrize::Bitmap& ImageJobWorker::getCurrent()
{
    return m_runner.getCurrent();
}

geometrize::Bitmap& ImageJobWorker::getTarget()
{
    return m_runner.getTarget();
}

const geometrize::Bitmap& ImageJobWorker::getCurrent() const
{
    return m_runner.getCurrent();
}

const geometrize::Bitmap& ImageJobWorker::getTarget() const
{
    return m_runner.getTarget();
}

geometrize::ImageRunner& ImageJobWorker::getRunner()
{
    return m_runner;
}

bool ImageJobWorker::isStepping() const
{
    return m_working;
}

}

}
