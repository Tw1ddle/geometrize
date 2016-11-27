#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "shape.h"

namespace geometrize
{

class ShapeFactory
{
public:
    static Shape create(ShapeType t, int xBound, int yBound)
    {

    }

    static Shape randomShape(int xBound, int yBound)
    {
        return create()
    }

    //static Shape randomShapeOf()
};

}

#endif // SHAPEFACTORY_H
