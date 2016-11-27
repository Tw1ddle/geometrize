#ifndef STATE_H
#define STATE_H

#include "bitmap/bitmapdata.h"
#include "shape/shape.h"

namespace geometrize
{

/**
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class State
{
public:
    /**
     * @brief Creates a new state.
     * @param shape A geometric primitive.
     * @param alpha The color alpha of the geometric shape.
     * @param target The target bitmap.
     * @param current The current bitmap.
     * @param buffer The buffer bitmap.
     */
    State(const Shape& shape, const int alpha, const BitmapData& target, const BitmapData& current, const BitmapData& buffer) :
        m_shape{shape}, m_score{-1}, m_alpha{alpha}, m_target{target}, m_current{current}, m_buffer{buffer} {}
    ~State() = default;
    State& operator=(const State&) = default;
    State(const State&) = default;

    /**
     * @brief Calculates a measure of the improvement drawing the primitive to the current bitmap will have.
     * The lower the energy, the better. The score is cached, set it to < 0 to recalculate it.
     * @return The energy measure.
     */
    inline float energy() const
    {
        if(score < 0) {
            score = Geometrize.energy(shape, alpha, target, current, buffer, score);
        }
        return score;
    }

    /**
     * @brief mutate Modifies the current state in a random fashion.
     * @return The old state - in case we want to go back to the old state.
     */
    inline State mutate()
    {
        State oldState(this);
        m_shape.mutate();
        return oldState;
    }

private:
    Shape m_shape; ///< The geometric primitive owned by the state.
    float m_score; ///< The score of the state, a measure of the improvement applying the state to the current bitmap will have.
    int m_alpha; ///< The alpha of the shape.

    BitmapData& m_target;
    BitmapData& m_current;
    BitmapData& m_buffer;
};

}

#endif // STATE_H
