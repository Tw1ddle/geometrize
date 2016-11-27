#ifndef IMAGERUNNER_H
#define IMAGERUNNER_H

#include <array>

#include "../model.h"
#include "../shape/shapetype.h"

namespace geometrize
{

/**
 * @brief The RunParams class encapsulates the parameters that may be passed to the runner.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct RunParams
{
    int primitiveCount = 0;
    std::array<ShapeType> = [];
};

/**
 * @brief The ImageRunner class is a helper class for creating a set of primitives from a single source image.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ImageRunner
{
public:
    ImageRunner(Bitmap& bitmap) : m_model{bitmap} {}
    ~ImageRunner() = default;
    ImageRunner& operator=(const ImageRunner&) = delete;
    ImageRunner(const ImageRunnera&) = delete;

    /**
     * @brief Updates runner until the number of primitives specified in the runner options are added.
     * @return An array containing data about the shapes added to the model in this step.
     */
    std::vector<ShapeResult> run()
    {
        std::vector<ShapeResult> results(options.primitiveCount);
        while(results.length <= options.primitiveCount)
        {
            std::vector<ShapeResult> shapes{m_model.step(options.shapeTypes, 128, 0)}; // TODO alpha, repeat etc
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
        return model.step(options.shapeTypes, 128, 0); // TODO alpha, repeat params from config etc
    }

    /**
     * @brief Gets the current bitmap with the primitives drawn on it.
     * @return The current bitmap.
     */
    BitmapData& getImageData()
    {
        return model.current;
    }

    /**
     * @brief Gets data about the shapes added to the model so far.
     * @return The shape results.
     */
    std::vector<ShapeResult> getShapeResults()
    {
        return model.shapeResults;
    }

private:
    Model m_model; ///< The model for the primitive optimization/fitting algorithm.
};

}

#endif // IMAGERUNNER_H
