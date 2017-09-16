#include "synchronousimagetask.h"

#include <vector>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/shaperesult.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/model.h"
#include "geometrize/shape/shapemutator.h"

#include "preferences/imagetaskpreferences.h"

namespace geometrize
{

namespace task
{

SynchronousImageTask::SynchronousImageTask(Bitmap& target) : m_runner{target}, m_preferences{}
{
}

SynchronousImageTask::SynchronousImageTask(Bitmap& target, Bitmap& background) : m_runner{target, background}, m_preferences{}
{
}

Bitmap& SynchronousImageTask::getTarget()
{
    return m_runner.getTarget();
}

Bitmap& SynchronousImageTask::getCurrent()
{
    return m_runner.getCurrent();
}

ShapeMutator& SynchronousImageTask::getShapeMutator()
{
    return m_runner.getModel().getShapeMutator();
}

std::vector<geometrize::ShapeResult> SynchronousImageTask::stepModel()
{
    return m_runner.step(m_preferences.getImageRunnerOptions());
}

geometrize::preferences::ImageTaskPreferences& SynchronousImageTask::getPreferences()
{
    return m_preferences;
}

void SynchronousImageTask::setPreferences(preferences::ImageTaskPreferences preferences)
{
    m_preferences = preferences;
}

}

}
