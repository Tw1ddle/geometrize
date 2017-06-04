#include "imagejobworker.h"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace job
{

ImageJobWorker::ImageJobWorker(Bitmap& bitmap) : QObject(), m_runner{bitmap}
{
}

ImageJobWorker::ImageJobWorker(Bitmap& bitmap, const Bitmap& initial) : QObject(), m_runner{bitmap, initial}
{
}

void ImageJobWorker::step(const geometrize::ImageRunnerOptions options)
{
    emit signal_willStep();
    const std::vector<geometrize::ShapeResult> results{m_runner.step(options)};
    emit signal_didStep(results);
}

geometrize::Bitmap& ImageJobWorker::getCurrent()
{
    return m_runner.getCurrent();
}

geometrize::Bitmap& ImageJobWorker::getTarget()
{
    return m_runner.getTarget();
}

geometrize::ImageRunner& ImageJobWorker::getRunner()
{
    return m_runner;
}

}

}
