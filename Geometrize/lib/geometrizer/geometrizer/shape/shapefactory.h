#pragma once

#include <bitset>
#include <memory>

#include "shape.h"
#include "circle.h"
#include "ellipse.h"
#include "rectangle.h"
#include "rotatedellipse.h"
#include "rotatedrectangle.h"
#include "shapetypes.h"
#include "triangle.h"
#include "../util.h"

namespace geometrize
{

class ShapeFactory
{
public:
    /**
     * @brief create Creates a new shape of the specified type.
     * @param t The type of shape to create.
     * @param xBound The x-bound of the whole canvas.
     * @param yBound The y-bound of the whole canvas.
     * @return The new shape.
     */
    static Shape* create(shapes::ShapeTypes t, const int xBound, const int yBound)
    {
        switch(t) {
            case shapes::ShapeTypes::CIRCLE:
                return new Circle(xBound, yBound);
            case shapes::ShapeTypes::ELLIPSE:
                return new Ellipse(xBound, yBound);
            case shapes::ShapeTypes::ROTATED_ELLIPSE:
                return new RotatedEllipse(xBound, yBound);
            case shapes::ShapeTypes::ROTATED_RECTANGLE:
                return new RotatedRectangle(xBound, yBound);
            case shapes::ShapeTypes::TRIANGLE:
                return new Triangle(xBound, yBound);
            case shapes::ShapeTypes::RECTANGLE:
                return new Rectangle(xBound, yBound);
            case shapes::ShapeTypes::SHAPE_COUNT:
             assert(0 && "Bad shape value");
        };

        assert(0 && "Unhandled shape type encountered");
        return new Rectangle(xBound, yBound);
    }

    /**
     * @brief randomShape Creates a random shape.
     * @param xBound The x-bound of the whole canvas.
     * @param yBound The y-bound of the whole canvas.
     * @return The new shape.
     */
    static Shape* randomShape(const int xBound, const int yBound)
    {
        return create(shapes::allShapes[util::Random::randomRange(0, static_cast<int>(shapes::allShapes.size()) - 1)], xBound, yBound);
    }

    /**
     * @brief randomShapeOf Creates a random shape from the types supplied.
     * @param t The types of shape to possibly create.
     * @param xBound The x-bound of the whole canvas.
     * @param yBound The y-bound of the whole canvas.
     * @return The new shape.
     */
    static Shape* randomShapeOf(const shapes::ShapeTypes t, const int xBound, const int yBound)
    {
        const std::bitset<32> b(t);
        std::vector<int> bits;
        for(unsigned int bit = 0; bit < b.count(); bit++) {
            if(bit) {
                bits.push_back(1 << bit);
            }
        }
        return create(static_cast<shapes::ShapeTypes>(bits[util::Random::randomRange(0, static_cast<int>(bits.size()))]), xBound, yBound);
    }
};

}
