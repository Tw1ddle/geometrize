#ifndef ROTATEDRECTANGLE_H
#define ROTATEDRECTANGLE_H

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The RotatedRectangle class represents a rotated rectangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class RotatedRectangle : public Shape
{
public:
    /**
     * @brief RotatedRectangle Creates a new rotated rectangle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    RotatedRectangle(const int xBound, const int yBound) : m_xBound(xBound), m_yBound(yBound)
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

    virtual shapes::ShapeTypes getType() const override
    {
        return shapes::ShapeTypes::ROTATED_RECTANGLE;
    }

private:
    const int m_xBound; ///< The x-bound of the whole canvas.
    const int m_yBound; ///< The y-bound of the whole canvas.
};

}

#endif // ROTATEDRECTANGLE_H
