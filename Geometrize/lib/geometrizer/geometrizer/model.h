#ifndef MODEL_H
#define MODEL_H

#include "model.h"
#include "bitmap/rgba.h"
#include "shape/shape.h"
#include "state.h"

namespace geometrize
{

/**
 * @brief The ShapeResult class is a container for info about a shape added to the model.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ShapeResult
{
public:
    ShapeResult() : score{0.0f}, color{0}, shape{nullptr} {}
    ShapeResult(const float score, const int color, Shape* shape) : score{score}, color{color}, shape{shape} {}
    ~ShapeResult() = default;
    ShapeResult& operator=(const ShapeResult&) = default;
    ShapeResult(const ShapeResult&) = default;

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
    Model(const BitmapData& target, const rgba backgroundColor) :
        m_target(target),
        m_current(target.getWidth(), target.getHeight(), backgroundColor),
        m_buffer(target.getWidth(), target.getHeight(), backgroundColor) {}

    Model() = delete;
    ~Model() = default;
    Model& operator=(const Model&) = delete;
    Model(const Model&) = delete;

    /**
     * @brief getWidth Gets the width of the target bitmap.
     * @return The width of the target bitmap.
     */
    std::size_t getWidth() const
    {
        return m_target.getWidth();
    }

    /**
     * @brief getHeight Gets the height of the target bitmap.
     * @return The height of the target bitmap.
     */
    std::size_t getHeight() const
    {
        return m_target.getHeight();
    }

    /**
     * @brief getAspectRatio Gets the aspect ratio of the target bitmap.
     * @return The aspect ratio of the target bitmap.
     */
    float getAspectRatio() const
    {
        if(m_target.getWidth() == 0 || m_target.getHeight() == 0) {
            return 0;
        }
        return static_cast<float>(m_target.getWidth()) / static_cast<float>(m_target.getHeight());
    }

    /**
     * @brief step Steps the primitive optimization/fitting algorithm.
     * @param shapeTypes The types of shape to use.
     * @param repeats How many times to repeat the stepping process with reduced search (per step), adding additional shapes.
     * @return A vector containing data about the shapes added to the model in this step.
     */
    std::vector<ShapeResult> step(const geometrize::shapes::ShapeTypes shapeTypes, const uint16_t, const std::size_t repeats)
    {
        // TODO
        //State state;
        std::vector<ShapeResult> results;
    }

    /**
     * @brief addShape Adds a shape to the model.
     * @param shape The shape to add.
     * @param alpha The alpha/opacity of the shape.
     * @return Data about the shape added to the model.
     */
    ShapeResult addShape(Shape& shape, const uint16_t alpha)
    {
        //BitmapData before = m_current.copyData();
        //std::vector<Scanline> scanlines = shape.rasterize();
        // TODO

        ShapeResult s(0, 0, &shape);

        return s;
    }

    /**
     * @brief getCurrent Gets the current bitmap.
     * @return The current bitmap.
     */
    BitmapData& getCurrent()
    {
        return m_current;
    }

    /**
     * @brief getShapeResults Gets the results data with info about the shapes added to the model so far.
     * @return The shape results data.
     */
    std::vector<ShapeResult>& getShapeResults()
    {
        return m_shapeResults;
    }

private:
    BitmapData m_target; ///< The target bitmap, the bitmap we aim to approximate.
    BitmapData m_current; ///< The current bitmap.
    BitmapData m_buffer; ///< Buffer bitmap.

    std::vector<ShapeResult> m_shapeResults; ///< The results data with info about the shapes added to the model so far.
};

}

#endif // MODEL_H
