#ifndef SHAPETYPE_H
#define SHAPETYPE_H

namespace geometrize
{

/**
 * @brief The ShapeType enum specifying the types of shapes that can be used. These can be combined to produce images composed of multiple primitive types.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
enum ShapeType
{
    RECTANGLE,
    ROTATED_RECTANGLE,
    TRIANGLE,
    ELLIPSE,
    ROTATED_ELLIPSE,
    CIRCLE,
    SHAPETYPE_MAX
};

}

#endif // SHAPETYPE_H
