#pragma once

#include <memory>
#include <vector>

#include <QObject>
#include <QString>

#include "geometrize/shaperesult.h"
#include "preferences/imagejobpreferences.h"

class QPixmap;

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{

namespace job
{
class ImageJobImpl;
}

}

namespace geometrize
{

namespace job
{

/**
 * @brief The ImageJob class contains the data worked on by an image job.
 */
class ImageJob : public QObject
{
    Q_OBJECT
public:
    ImageJob(const std::string& displayName, const std::string& jobUrl, Bitmap& bitmap);
    ImageJob& operator=(const ImageJob&) = delete;
    ImageJob(const ImageJob&) = delete;
    ~ImageJob() = default;

    /**
     * @brief getDisplayName Gets the display name of the image job.
     * @return The display name of the image job.
     */
    std::string getDisplayName() const;

    /**
     * @brief getJobUrl Gets the URL of the image job.
     * @return The URL of the image job.
     */
    std::string getJobUrl() const;

    /**
     * @brief getJobId Gets the unique id of the image job.
     * @return The unique id of the image job.
     */
     int getJobId() const;

     /**
      * @brief getBitmap Gets a reference to the bitmap data the image job is working on.
      * @return A reference to the bitmap data of the image job.
      */
     Bitmap& getBitmap();

     /**
      * @brief stepModel Steps the internal model, typically adding a shape.
      */
     void stepModel();

     /**
      * @brief applyPreferences Applies the given image job preferences to the job.
      * @param preferences The preferences to apply.
      */
     void applyPreferences(const geometrize::preferences::ImageJobPreferences& preferences);

     /**
      * @brief getPreferences Gets a reference to the current preferences of this job.
      * @return A reference to the current preferences of this job.
      */
     geometrize::preferences::ImageJobPreferences& getPreferences();

signals:
     /**
      * @brief signal_modelWillStep Signal that is emitted immediately before the underlying image job model is stepped.
      */
     void signal_modelWillStep();

     /**
      * @brief signal_modelDidStep Signal that is emitted immediately after the underlying image job model is stepped.
      * @param shapes The shapes that were added in the last step.
      */
     void signal_modelDidStep(const std::vector<geometrize::ShapeResult>& shapes);

private:
    class ImageJobImpl;
    std::unique_ptr<ImageJobImpl> d;
};

}

}
