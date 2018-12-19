#pragma once

#include <memory>
#include <string>
#include <vector>

#include <QObject>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/shaperesult.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/shape/shapemutator.h"

#include "preferences/imagetaskpreferences.h"

namespace geometrize
{

namespace script
{
class GeometrizerEngine;
}

}

namespace chaiscript
{
class ChaiScript;
}

Q_DECLARE_METATYPE(std::vector<geometrize::ShapeResult>) ///< Shape data passed around by the image task worker thread.
Q_DECLARE_METATYPE(geometrize::ImageRunnerOptions) ///< Image runner options passed to the image task worker thread.
Q_DECLARE_METATYPE(std::shared_ptr<geometrize::Shape>) ///< Shape passed to the image task worker thread.
Q_DECLARE_METATYPE(geometrize::rgba) ///< Shape color passed to the image task worker thread.

namespace geometrize
{

namespace task
{

/**
 * @brief The ImageTask class transforms a source image into a collection of shapes approximating the source image.
 */
class ImageTask : public QObject
{
    Q_OBJECT
public:
    ImageTask(Bitmap& target, Qt::ConnectionType workerConnectionType = Qt::AutoConnection);
    ImageTask(Bitmap& target, Bitmap& background, Qt::ConnectionType workerConnectionType = Qt::AutoConnection);
    ImageTask(const std::string& displayName, Bitmap& bitmap, Qt::ConnectionType workerConnectionType = Qt::AutoConnection);
    ImageTask(const std::string& displayName, Bitmap& bitmap, const Bitmap& initial, Qt::ConnectionType workerConnectionType = Qt::AutoConnection);

    ImageTask& operator=(const ImageTask&) = delete;
    ImageTask(const ImageTask&) = delete;
    virtual ~ImageTask();

    /**
     * @brief getGeometrizer Gets a reference to the script-based engine used to turn images into shapes.
     * @return The script-based engine used to turn images into shapes.
     */
    geometrize::script::GeometrizerEngine& getGeometrizer();

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
     * @brief getTarget Gets the target bitmap, const-edition.
     * @return The target bitmap.
     */
    const Bitmap& getTarget() const;

    /**
     * @brief getCurrent Gets the current bitmap, const-edition.
     * @return The current bitmap.
     */
    const Bitmap& getCurrent() const;

    /**
     * @brief getWidth Gets the width of the images used by the image task.
     * @return The width of the images used by the image task.
     */
    std::uint32_t getWidth() const;

    /**
     * @brief getHeight Gets the height of the images used by the image task.
     * @return The height of the images used by the image task.
     */
    std::uint32_t getHeight() const;

    /**
     * @brief getDisplayName Gets the display name of the image task.
     * @return The display name of the image task.
     */
    std::string getDisplayName() const;

    /**
     * @brief getTaskId Gets the unique id of the image task.
     * @return The unique id of the image task.
     */
     std::size_t getTaskId() const;

     /**
      * @brief isStepping Returns true if the internal model is currently stepping.
      * @return True if the internal model is currently stepping, else false.
      */
     bool isStepping() const;

     /**
      * @brief stepModel Steps the internal model, typically adding a shape.
      */
     void stepModel();

     /**
      * @brief drawShape Draws a shape with the given color to the internal model.
      * @param shape The shape to add to the model.
      * @param color The color of the shape.
      */
     void drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color);

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

signals:
     /**
      * @brief signal_step Signal that the image task emits to make the internal model step.
      */
     void signal_step(geometrize::ImageRunnerOptions options);

     /**
      * @brief signal_drawShape Signal that the image task emits to draw a shape to the internal model.
      * @param shape The shape to draw.
      * @param color The color of the shape to draw.
      */
     void signal_drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color);

     /**
      * @brief signal_modelWillStep Signal that is emitted immediately before the underlying image task model is stepped.
      */
     void signal_modelWillStep();

     /**
      * @brief signal_modelDidStep Signal that is emitted immediately after the underlying image task model is stepped.
      * @param shapes The shapes that were added in the last step.
      */
     void signal_modelDidStep(std::vector<geometrize::ShapeResult> shapes);

     /**
      * @brief signal_preferencesSet Signal that is emitted immediately after the image task preferences are set.
      */
     void signal_preferencesSet();

private:
    void modelWillStep();
    void modelDidStep(std::vector<geometrize::ShapeResult> shapes);

    class ImageTaskImpl;
    std::unique_ptr<ImageTaskImpl> d;
};

}

}
