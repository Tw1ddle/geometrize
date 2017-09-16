#pragma once

#include <vector>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/shaperesult.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/shape/shapemutator.h"

#include "preferences/imagetaskpreferences.h"

namespace geometrize
{

namespace task
{

/**
 * @brief The SynchronousImageTask class transforms a source image into a collection of shapes approximating the source image.
 * Unlike the ImageTask class, it returns shape results directly when stepped, rather than doing the work in another thread.
 * Note that the shape stepping is still asynchronous multithreaded, but the direct calls on this class are synchronous.
 * This is essentially a simple implementation for use in scripts and console programs.
 */
class SynchronousImageTask
{
public:
    SynchronousImageTask(Bitmap& target);
    SynchronousImageTask(Bitmap& target, Bitmap& background);

    SynchronousImageTask& operator=(const SynchronousImageTask&) = delete;
    SynchronousImageTask(const SynchronousImageTask&) = delete;
    ~SynchronousImageTask() = default;

    /**
     * @brief getTarget Gets the target bitmap.
     * @return The target bitmap.
     */
    Bitmap& getTarget();

    /**
     * @brief getCurrent Gets the current bitmap.
     * @return The current bitmap.
     */
    Bitmap& getCurrent();

    /**
     * @brief getShapeMutator Gets the current shape mutator.
     * @return The current shape mutator.
     */
    ShapeMutator& getShapeMutator();

     /**
      * @brief stepModel Steps the internal model, typically adding a shape.
      * @return A vector of shape results produced when stepping the model.
      */
     std::vector<geometrize::ShapeResult> stepModel();

     /**
      * @brief getPreferences Gets a reference to the current preferences of this task.
      * @return A reference to the current preferences of this task.
      */
     geometrize::preferences::ImageTaskPreferences& getPreferences();

     /**
      * @brief setPreferences Sets the preferences for this task.
      * @param preferences The preferences to set.
      */
     void setPreferences(preferences::ImageTaskPreferences preferences);

private:
    geometrize::ImageRunner m_runner;
    preferences::ImageTaskPreferences m_preferences;
};

}

}
