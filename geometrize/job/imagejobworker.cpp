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

void ImageJobWorker::step()
{
    emit signal_willStep();
    const std::vector<geometrize::ShapeResult> results{m_runner.step()};
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

}

}
