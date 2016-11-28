#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "shape.h"

namespace geometrize
{

class ShapeFactory
{
public:
    /**
     * @brief create
     * @param t
     * @param xBound
     * @param yBound
     * @return
     */
    static Shape create(ShapeTypes t, int xBound, int yBound)
    {

    }


    /**
     * @brief randomShape Creates a random shape.
     * @param xBound The x-bound of the whole canvas.
     * @param yBound The y-bound of the whole canvas.
     * @return The new shape.
     */
    static Shape randomShape(int xBound, int yBound)
    {
        return create();// TODO
    }
};

}

#endif // SHAPEFACTORY_H
