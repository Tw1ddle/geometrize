#pragma once

#include "model.h"
#include "bitmap/rgba.h"
#include "shape/shape.h"
#include "state.h"

namespace geometrize
{

/**
 * @brief The ShapeResult struct is a container for info about a shape added to the model.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct ShapeResult
{
    float score;
    rgba color;
    Shape* shape;
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
        m_buffer(target.getWidth(), target.getHeight(), backgroundColor),
        m_score{geometrize::core::differenceFull(m_target, m_current)}
    {}

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
     * @param alpha The alpha of the shape.
     * @param repeats How many times to repeat the stepping process with reduced search (per step), adding additional shapes.
     * @return A vector containing data about the shapes added to the model in this step.
     */
    std::vector<ShapeResult> step(const geometrize::shapes::ShapeTypes shapeTypes, const uint16_t alpha, const int repeats)
    {
        State state{geometrize::core::bestHillClimbState(shapeTypes, alpha, 1000, 100, 16, m_target, m_current, m_buffer)}; // TODO
        std::vector<ShapeResult> results;
        results.push_back(addShape(state.m_shape, alpha));

        for(int i = 0; i <= repeats; i++) {
            const float before{state.calculateEnergy(m_target, m_current, m_buffer)};
            state = geometrize::core::hillClimb(state, 100, m_target, m_current, m_buffer);
            const float after{state.calculateEnergy(m_target, m_current, m_buffer)};
            if(before == after) {
                break;
            }
            results.push_back(addShape(state.m_shape, state.m_alpha));
        }
        return results;
    }

    /**
     * @brief addShape Adds a shape to the model.
     * @param shape The shape to add.
     * @param alpha The alpha/opacity of the shape.
     * @return Data about the shape added to the model.
     */
    ShapeResult addShape(Shape* shape, const uint16_t alpha)
    {
        const BitmapData before{m_current};
        const std::vector<Scanline> lines{shape->rasterize()};
        const rgba color{geometrize::core::computeColor(m_target, m_current, lines, alpha)};
        geometrize::core::drawLines(m_current, color, lines);

        m_score = geometrize::core::differencePartial(m_target, before, m_current, m_score, lines);

        ShapeResult result{m_score, color, shape};
        m_shapeResults.push_back(result);

        return result;
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
    float m_score; ///< Score derived from calculating the difference between bitmaps.
    std::vector<ShapeResult> m_shapeResults; ///< The results data with info about the shapes added to the model so far.
};

}
