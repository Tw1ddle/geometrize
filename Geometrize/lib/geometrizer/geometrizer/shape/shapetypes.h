#ifndef SHAPETYPE_H
#define SHAPETYPE_H

namespace geometrize
{

/**
 * @brief The ShapeTypes enum specifies the types of shapes that can be used. These can be combined to produce images composed of multiple primitive types.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
enum class ShapeTypes
{
    RECTANGLE = 1,
    ROTATED_RECTANGLE = 2,
    TRIANGLE = 4,
    ELLIPSE = 8,
    ROTATED_ELLIPSE = 16,
    CIRCLE = 32,
    SHAPE_COUNT = 6
};

}

#endif // SHAPETYPE_H
