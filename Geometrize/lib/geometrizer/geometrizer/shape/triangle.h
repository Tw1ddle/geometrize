#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The Triangle class represents a triangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Triangle : public Shape
{
public:
    /**
     * @brief Triangle Creates a new triangle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Triangle(const int xBound, const int yBound) : m_xBound(xBound), m_yBound(yBound)
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
        return shapes::ShapeTypes::TRIANGLE;
    }

private:
    const int m_xBound; ///< The x-bound of the whole canvas.
    const int m_yBound; ///< The y-bound of the whole canvas.
};

}

#endif // TRIANGLE_H
