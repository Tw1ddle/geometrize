#pragma once

#include <memory>

#include <QObject>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace job
{

/**
 * @brief The ImageJobWorker class transforms a source image into a collection of shapes. Made to be run on a separate thread to avoid blocking UI.
 */
class ImageJobWorker : public QObject
{
    Q_OBJECT
public:
    explicit ImageJobWorker(Bitmap& bitmap);
    ImageJobWorker(Bitmap& bitmap, const Bitmap& initial);
    ImageJobWorker& operator=(const ImageJobWorker&) = delete;
    ImageJobWorker(const ImageJobWorker&) = delete;
    ~ImageJobWorker();

    /**
     * @brief step Steps the image job worker. Emits the willStep signal when called, and didStep signal on completion.
     * @param options The options to provide the image runner when stepping.
     */
    void step(geometrize::ImageRunnerOptions options);

    /**
     * @brief isStepping Returns true if the internal model is currently stepping.
     * @return True if the internal model is currently stepping, else false.
     */
    bool isStepping() const;

    /**
     * @brief drawShape Draws a shape with the given color to the image job. Emits the willStep signal when called, and didStep signal on completion.
     * @param shape The shape to draw.
     * @param color The color of the shape to draw.
     */
    void drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color);

    /**
     * @brief getCurrent Gets the current working bitmap.
     * @return The current working bitmap.
     */
    geometrize::Bitmap& getCurrent();

    /**
     * @brief getTarget Gets the current target bitmap.
     * @return The current target bitmap.
     */
    geometrize::Bitmap& getTarget();

    ImageRunner& getRunner();

signals:
    void signal_willStep();
    void signal_didStep(std::vector<geometrize::ShapeResult> shapes);

private:
    ImageRunner m_runner;
    std::atomic_bool m_working;
};

}

}
