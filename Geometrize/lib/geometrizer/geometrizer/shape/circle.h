#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The Circle class represents a circle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Circle : public Shape
{
public:
    /**
     * @brief Circle Creates a new circle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Circle(const int xBound, const int yBound) : m_xBound(xBound), m_yBound(yBound)
    {
        // TODO
    }

    virtual std::vector<Scanline> rasterize() const override
    {
        std::vector<Scanline> lines;
        return lines; // TODO
    }

    virtual void mutate() override
    {
        // TODO
    }

private:
    const int m_xBound; ///< The x-bound of the whole canvas.
    const int m_yBound; ///< The y-bound of the whole canvas.
};

}

#endif // CIRCLE_H
