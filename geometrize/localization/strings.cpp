#include "strings.h"

namespace geometrize
{

namespace strings
{

QString Strings::getApplicationName()
{
    return tr("Geometrize", "This means to 'turn into geometry' or to 'represent using geometric primitives'. The name of the application, so must be short and meaningful. Do not translate if this is difficult to communicate in one word.");
}

QString Strings::getShortApplicationDescription()
{
    return tr("Geometrize recreates your photos using simple geometric shapes, transforming them into unique artwork.", "This is a short description for what this application is and what it does.");
}

QString Strings::getCopyrightNotice()
{
    return tr("Copyright 2015-2021 Sam Twidale", "This is the copyright notice of the application.");
}

QString Strings::getShapeTypeNamePlural(const geometrize::ShapeTypes shapeType)
{
    switch(shapeType) {
    case geometrize::ShapeTypes::RECTANGLE:
        return tr("Rectangles", "Plural form of rectangle geometric shape");
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        return tr("Rotated Rectangles", "Plural form of rectangle geometric shape. These rectangles are rotated through an angle (0-360 degrees)");
    case geometrize::ShapeTypes::TRIANGLE:
        return tr("Triangles", "Plural form of triangle geometric shape");
    case geometrize::ShapeTypes::ELLIPSE:
        return tr("Ellipses", "Plural form of ellipse/ellipsoid geometric shape");
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        return tr("Rotated Ellipses", "Plural form of ellipse geometric shape. These ellipses are rotated through an angle (0-360 degrees)");
    case geometrize::ShapeTypes::CIRCLE:
        return tr("Circles", "Plural form of circle/circular geometric shape");
    case geometrize::ShapeTypes::LINE:
        return tr("Lines", "Plural form of the simple line geometric shape");
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        return tr("Quadratic Beziers", "Plural form of quadratic bezier curve geometric shape");
    case geometrize::ShapeTypes::POLYLINE:
        return tr("Polylines", "Plural form of the polyline geometric shape (multiple line segments joined together)");
    default:
        return tr("Unknown Shapes", "Plural form of an unknown type of shapes");
    }
}

}

}
