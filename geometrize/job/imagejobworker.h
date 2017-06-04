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
 * @brief The ImageJobWorker class transforms a source image into a collection of shapes. Made to be run on a separate thread to avoid blocking.
 */
class ImageJobWorker : public QObject
{
    Q_OBJECT
public:
    explicit ImageJobWorker(Bitmap& bitmap);
    ImageJobWorker(Bitmap& bitmap, const Bitmap& initial);
    ImageJobWorker& operator=(const ImageJobWorker&) = delete;
    ImageJobWorker(const ImageJobWorker&) = delete;
    ~ImageJobWorker() = default;

    void step(geometrize::ImageRunnerOptions options);

    geometrize::Bitmap& getCurrent();
    geometrize::Bitmap& getTarget();
    ImageRunner& getRunner();

signals:
    void signal_willStep();
    void signal_didStep(std::vector<geometrize::ShapeResult> shapes);

private:
    ImageRunner m_runner;
};

}

}
