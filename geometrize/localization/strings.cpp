#include "strings.h"

namespace geometrize
{

namespace strings
{

QString Strings::getApplicationName()
{
    return tr("Geometrize", "This means to 'turn into geometry' or to 'represent using geometric primitives'. The name of the application, so must be short and meaningful. Do not translate if this is difficult to communicate in one word.");
}

QString Strings::getApplicationDescription()
{
    return tr("A tool for reproducing media as animated geometric primitives.", "This is a short description of what this application does.");
}

QString Strings::getCopyrightNotice()
{
    return tr("Copyright 2015-2017 Sam Twidale", "This is the copyright notice of the application.");
}

QString Strings::getShapeTypeNameSingular(const geometrize::ShapeTypes shapeType)
{
    switch(shapeType) {
    case geometrize::ShapeTypes::RECTANGLE:
        return tr("Rectangle");
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        return tr("Rotated Rectangle");
    case geometrize::ShapeTypes::TRIANGLE:
        return tr("Triangle");
    case geometrize::ShapeTypes::ELLIPSE:
        return tr("Ellipse");
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        return tr("Rotated Ellipse");
    case geometrize::ShapeTypes::CIRCLE:
        return tr("Circle");
    case geometrize::ShapeTypes::LINE:
        return tr("Line");
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        return tr("Quadratic Bezier");
    case geometrize::ShapeTypes::POLYLINE:
        return tr("Polyline");
    default:
        return tr("Unknown Shape");
    }
}

QString Strings::getShapeTypeNamePlural(const geometrize::ShapeTypes shapeType)
{
    switch(shapeType) {
    case geometrize::ShapeTypes::RECTANGLE:
        return tr("Rectangles");
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        return tr("Rotated Rectangles");
    case geometrize::ShapeTypes::TRIANGLE:
        return tr("Triangles");
    case geometrize::ShapeTypes::ELLIPSE:
        return tr("Ellipses");
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        return tr("Rotated Ellipses");
    case geometrize::ShapeTypes::CIRCLE:
        return tr("Circles");
    case geometrize::ShapeTypes::LINE:
        return tr("Lines");
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        return tr("Quadratic Beziers");
    case geometrize::ShapeTypes::POLYLINE:
        return tr("Polylines");
    default:
        return tr("Unknown Shapes");
    }
}

}

}
