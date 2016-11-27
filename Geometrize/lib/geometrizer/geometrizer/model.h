#ifndef MODEL_H
#define MODEL_H

#include "model.h"
#include "shape/shape.h"

namespace geometrize
{

/**
 * @brief The ShapeResult class is a container for info about a shape added to the model.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ShapeResult
{
public:
    ShapeResult(const float score, const int color, Shape* shape) : score{score}, color{color}, shape{shape} {}
    ~ShapeResult() = default;
    ShapeResult& operator=(const ShapeResult&) = delete;
    ShapeResult(const ShapeResult&) = delete;

    const float score;
    const int color;
    const Shape* const shape;
};

/**
 * @brief The Model class is the model for the primitive optimization/fitting algorithm.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Model
{
public:
    Model();
    ~Model() = default;
    Model& operator=(const Model&) = delete;
    Model(const Model&) = delete;

private:

};

}

#endif // MODEL_H
