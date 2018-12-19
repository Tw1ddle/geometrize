#pragma once

#include <memory>
#include <vector>

#include <QObject>

#include "preferences/imagetaskpreferences.h"

namespace geometrize
{
class Bitmap;
struct ShapeResult;
}

namespace geometrize
{

namespace task
{

/**
 * @brief The SynchronousImageTask class transforms a source image into a collection of shapes approximating the source image.
 * A wrapper around the asynchronous ImageTask class, this blocks when stepping the model.
 * This is a convenience class for use in scripts and console programs where we would rather block/wait when geometrizing something.
 */
class SynchronousImageTask : public QObject
{
    Q_OBJECT
public:
    SynchronousImageTask(Bitmap& target);
    virtual ~SynchronousImageTask();
    SynchronousImageTask& operator=(const SynchronousImageTask&) = delete;
    SynchronousImageTask(const SynchronousImageTask&) = delete;

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
      * @brief stepModel Steps the internal model, typically adding a shape.
      */
     void stepModel();

     /**
      * @brief drawBackgroundRectangle Convenience function that draws a background rectangle shape using the target image's background color.
      */
     void drawBackgroundRectangle();

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

     /**
      * @brief getShapes Gets a vector of shapes generated so far by this task.
      * @return The vector of shapes generated so far by this task.
      */
     std::vector<geometrize::ShapeResult> getShapes() const;

private:
     class SynchronousImageTaskImpl;
     std::unique_ptr<SynchronousImageTaskImpl> d;
};

}

}
