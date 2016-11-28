#ifndef IMAGERUNNER_H
#define IMAGERUNNER_H

#include <array>

#include "../bitmap/bitmapdata.h"
#include "../model.h"
#include "../shape/shapetypes.h"

namespace geometrize
{

/**
 * @brief The RunParams class encapsulates the parameters that may be passed to the runner.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct RunParams
{
    int primitiveCount = 50;
    ShapeTypes shapeTypes = ShapeTypes::RECTANGLE;
};

/**
 * @brief The ImageRunner class is a helper class for creating a set of primitives from a single source image.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ImageRunner
{
public:
    ImageRunner(BitmapData& bitmap) :  m_model{bitmap, geometrize::getAverageImageColor(bitmap)} {} // TODO set the starting color optionally
    ~ImageRunner() = default;
    ImageRunner& operator=(const ImageRunner&) = delete;
    ImageRunner(const ImageRunner&) = delete;

    /**
     * @brief Updates runner until the number of primitives specified in the runner options are added.
     * @return An array containing data about the shapes added to the model in this step.
     */
    std::vector<ShapeResult> run()
    {
        std::vector<ShapeResult> results(m_options.primitiveCount);
        while(results.size() <= m_options.primitiveCount) {
            std::vector<ShapeResult> shapes{m_model.step(m_options.shapeTypes, 128, 0)}; // TODO alpha, repeat etc
            std::move(shapes.begin(), shapes.end(), std::back_inserter(results));
        }
        return results;
    }

    /**
     * @brief Updates the model once.
     * @return An array containing data about the shapes just added to the model.
     */
    std::vector<ShapeResult> step()
    {
        return m_model.step(m_options.shapeTypes, 128, 0); // TODO alpha, repeat params from config etc
    }

    /**
     * @brief Gets the current bitmap with the primitives drawn on it.
     * @return The current bitmap.
     */
    const BitmapData& getImageData()
    {
        return m_model.getCurrent();
    }

    /**
     * @brief Gets data about the shapes added to the model so far.
     * @return The shape results.
     */
    std::vector<ShapeResult>& getShapeResults()
    {
        return m_model.getShapeResults();
    }

private:
    Model m_model; ///< The model for the primitive optimization/fitting algorithm.
    RunParams m_options; ///< The runtime configuration parameters for the runner.
};

}

#endif // IMAGERUNNER_H
