#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The Rectangle class represents a rectangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Rectangle : public Shape
{
public:
    /**
     * @brief Rectangle Creates a new rectangle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Rectangle(const int xBound, const int yBound) : m_xBound(xBound), m_yBound(yBound)
    {
        m_x1 = util::Random::randomRange(0, m_xBound);
        m_y1 = util::Random::randomRange(0, m_yBound);
        m_x2 = util::clamp(m_x1 + util::Random::randomRange(0, util::Random::randomRange(0, 32) + 1), 0, m_xBound);
        m_y2 = util::clamp(m_y1 + util::Random::randomRange(0, util::Random::randomRange(0, 32) + 1), 0, m_yBound);
    }

    virtual std::vector<Scanline> rasterize() const override
    {
        std::vector<Scanline> lines;
        for(int y = m_y1; y < m_y2; y++) {
            if(m_x1 != m_x2) {
                lines.push_back(Scanline(y, std::min(m_x1, m_x2), std::max(m_x1, m_x2), 0xFFFF));
            }
        }
        return lines;
    }

    virtual void mutate() override
    {
        const int r = util::Random::randomRange(0, 1);

        switch(r) {
        case 0:
            m_x1 = util::clamp(m_x1 + util::Random::randomRange(-16, 16), 0, m_xBound - 1);
            m_y1 = util::clamp(m_y1 + util::Random::randomRange(-16, 16), 0, m_yBound - 1);
        case 1:
            m_x2 = util::clamp(m_x2 + util::Random::randomRange(-16, 16), 0, m_xBound - 1);
            m_y2 = util::clamp(m_y2 + util::Random::randomRange(-16, 16), 0, m_yBound - 1);
        }
    }

private:
    const int m_xBound; ///< The x-bound of the whole canvas.
    const int m_yBound; ///< The y-bound of the whole canvas.

    int m_x1; ///< Left coordinate.
    int m_y1; ///< Top coordinate.
    int m_x2; ///< Right coordinate.
    int m_y2; ///< Bottom coordinate.
};

}

#endif // RECTANGLE_H
