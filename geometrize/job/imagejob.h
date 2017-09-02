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

#include "preferences/imagejobpreferences.h"

Q_DECLARE_METATYPE(std::vector<geometrize::ShapeResult>) ///< Shape data passed around by the image job worker thread.
Q_DECLARE_METATYPE(geometrize::ImageRunnerOptions) ///< Image runner options passed to the image job worker thread.
Q_DECLARE_METATYPE(std::shared_ptr<geometrize::Shape>) ///< Shape passed to the image job worker thread.
Q_DECLARE_METATYPE(geometrize::rgba) ///< Shape color passed to the image job worker thread.

namespace geometrize
{

namespace job
{

/**
 * @brief The ImageJob class transforms a source image into a collection of shapes approximating the source image.
 */
class ImageJob : public QObject
{
    Q_OBJECT
public:
    ImageJob(Bitmap& target);
    ImageJob(Bitmap& target, Bitmap& background);
    ImageJob(const std::string& displayName, Bitmap& bitmap);
    ImageJob(const std::string& displayName, Bitmap& bitmap, const Bitmap& initial);

    ImageJob& operator=(const ImageJob&) = delete;
    ImageJob(const ImageJob&) = delete;
    ~ImageJob();

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
     * @brief getDisplayName Gets the display name of the image job.
     * @return The display name of the image job.
     */
    std::string getDisplayName() const;

    /**
     * @brief getJobId Gets the unique id of the image job.
     * @return The unique id of the image job.
     */
     std::size_t getJobId() const;

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
      * @brief getPreferences Gets a reference to the current preferences of this job.
      * @return A reference to the current preferences of this job.
      */
     geometrize::preferences::ImageJobPreferences& getPreferences();

     /**
      * @brief setPreferences Sets the preferences for this job.
      * @param preferences The preferences to set.
      */
     void setPreferences(preferences::ImageJobPreferences preferences);

signals:
     /**
      * @brief signal_step Signal that the image job emits to make the internal model step.
      */
     void signal_step(geometrize::ImageRunnerOptions options);

     /**
      * @brief signal_drawShape Signal that the image job emits to draw a shape to the internal model.
      * @param shape The shape to draw.
      * @param color The color of the shape to draw.
      */
     void signal_drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color);

     /**
      * @brief signal_modelWillStep Signal that is emitted immediately before the underlying image job model is stepped.
      */
     void signal_modelWillStep();

     /**
      * @brief signal_modelDidStep Signal that is emitted immediately after the underlying image job model is stepped.
      * @param shapes The shapes that were added in the last step.
      */
     void signal_modelDidStep(std::vector<geometrize::ShapeResult> shapes);

private:
    void modelWillStep();
    void modelDidStep(std::vector<geometrize::ShapeResult> shapes);

    class ImageJobImpl;
    std::unique_ptr<ImageJobImpl> d;
};

}

}
