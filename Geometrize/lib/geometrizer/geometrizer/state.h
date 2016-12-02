#ifndef STATE_H
#define STATE_H

#include <memory>

#include "bitmap/bitmapdata.h"
#include "geometrize.h"
#include "shape/shape.h"
#include "shape/shapefactory.h"
#include "shape/shapetypes.h"

namespace geometrize
{

namespace core
{
    // Forward declare energy function
    // TODO
    float energy(const std::vector<Scanline>& lines, const int alpha, const BitmapData& target, const BitmapData& current, BitmapData& buffer, const float score);
}

/**
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class State
{
public:
    /**
     * @brief Creates a new state.
     * @param shapeTypes The types of geometric primitive to select.
     * @param alpha The color alpha of the geometric shape.
     * @param target The target bitmap.
     * @param current The current bitmap.
     * @param buffer The buffer bitmap.
     */
    State(const shapes::ShapeTypes shapeTypes, const int alpha, const int width, const int height) :
        m_score{-1.0f}, m_alpha{alpha}, m_shape{ShapeFactory::randomShapeOf(shapeTypes, width, height)}
    {}

    ~State() = default;
    State& operator=(const State& other) = default;
    State(const State& other) = default;

    /**
     * @brief Calculates a measure of the improvement drawing the primitive to the current bitmap will have.
     * The lower the energy, the better. The score is cached, set it to < 0 to recalculate it.
     * @return The energy measure.
     */
    inline float calculateEnergy(const BitmapData& target, const BitmapData& current, BitmapData& buffer)
    {
        if(m_score < 0) {
            m_score = geometrize::core::energy(m_shape->rasterize(), m_alpha, target, current, buffer, m_score); // TODO
        }
        return m_score;
    }

    /**
     * @brief mutate Modifies the current state in a random fashion.
     * @return The old state - in case we want to go back to the old state.
     */
    inline State mutate()
    {
        State oldState(*this);
        m_shape->mutate();
        return oldState;
    }

    Shape* m_shape; ///< The geometric primitive owned by the state.
    float m_score; ///< The score of the state, a measure of the improvement applying the state to the current bitmap will have.
    int m_alpha; ///< The alpha of the shape.
};

}

#endif // STATE_H
