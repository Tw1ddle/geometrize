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
    return tr("Geometrize recreates your photos using simple geometric shapes, transforming them into unique artwork.", "This is a short description of what this application is and does.");
}

QString Strings::getLongApplicationDescription()
{
    return tr("Geometrize transforms images into shapes, producing unique artistic effects."
    "Any image it is given gets reconstructed using simple shapes like rectangles, triangles, lines and ellipses, one geometric primitive at a time."
    "\n"
    "The resulting geometric primitives are inherently vector based, and are used to produce infinitely scalable images without loss of quality."
    "Geometrize saves the results as SVGs, regular JPEG and PNG images, animated GIFs, and even as canvas/WebGL webpages."
    "\n"
    "Geometrize comes bundled with hundreds of curated examples, complete with pre-tuned settings."
    "All of the key features and usages discovered are covered by comprehensive video tutorials."
    "The program gives you full creative control over the geometrization process, with intuitive settings for the types of geometric primitive to use, shape opacity, areas to focus, and more.",
    "This is a long description of what this application is and does.");
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
