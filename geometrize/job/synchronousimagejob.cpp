#include "synchronousimagejob.h"

#include <vector>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/shaperesult.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/model.h"
#include "geometrize/shape/shapemutator.h"

#include "preferences/imagejobpreferences.h"

namespace geometrize
{

namespace job
{

SynchronousImageJob::SynchronousImageJob(Bitmap& target) : m_runner{target}, m_preferences{}
{
}

SynchronousImageJob::SynchronousImageJob(Bitmap& target, Bitmap& background) : m_runner{target, background}, m_preferences{}
{
}

Bitmap& SynchronousImageJob::getTarget()
{
    return m_runner.getTarget();
}

Bitmap& SynchronousImageJob::getCurrent()
{
    return m_runner.getCurrent();
}

ShapeMutator& SynchronousImageJob::getShapeMutator()
{
    return m_runner.getModel().getShapeMutator();
}

std::vector<geometrize::ShapeResult> SynchronousImageJob::stepModel()
{
    return m_runner.step(m_preferences.getImageRunnerOptions());
}

geometrize::preferences::ImageJobPreferences& SynchronousImageJob::getPreferences()
{
    return m_preferences;
}

void SynchronousImageJob::setPreferences(preferences::ImageJobPreferences preferences)
{
    m_preferences = preferences;
}

}

}
