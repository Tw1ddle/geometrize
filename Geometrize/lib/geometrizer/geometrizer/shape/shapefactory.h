#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "shape.h"
#include "shapetypes.h"
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
    static Shape create(ShapeTypes t, const int xBound, const int yBound)
    {
        // TODO
        switch(t) {
            case ShapeTypes.RECTANGLE:
            return
        };
    }

    /**
     * @brief randomShape Creates a random shape.
     * @param xBound The x-bound of the whole canvas.
     * @param yBound The y-bound of the whole canvas.
     * @return The new shape.
     */
    static Shape randomShape(const int xBound, const int yBound)
    {
        return create(1 << geometrize::util::Random::randomRange(0, ShapeTypes::SHAPE_COUNT - 1), xBound, yBound);
    }

    /**
     * @brief randomShapeOf Creates a random shape from the types supplied.
     * @param t The types of shape to possibly create.
     * @param xBound The x-bound of the whole canvas.
     * @param yBound The y-bound of the whole canvas.
     * @return The new shape.
     */
    static Shape randomShapeOf(const ShapeTypes t, const int xBound, const int yBound)
    {
        // TODO
    }
};

}

#endif // SHAPEFACTORY_H
