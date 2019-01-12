#include "areaofinfluenceshape.h"

#include <cassert>
#include <utility>
#include <vector>

#include "geometrize/shape/circle.h"
#include "geometrize/shape/ellipse.h"
#include "geometrize/shape/line.h"
#include "geometrize/shape/polyline.h"
#include "geometrize/shape/quadraticbezier.h"
#include "geometrize/shape/rectangle.h"
#include "geometrize/shape/rotatedellipse.h"
#include "geometrize/shape/rotatedrectangle.h"
#include "geometrize/shape/triangle.h"
#include "geometrize/shape/shapemutator.h"
#include "geometrize/rasterizer/rasterizer.h"
#include "geometrize/rasterizer/scanline.h"

namespace geometrize
{

namespace scene
{

void AreaOfInfluenceShape::setup(const int x, const int y, const geometrize::ShapeTypes type)
{
    // Just create an entire new shape every time (since that isn't very expensive)
    switch(type) {
    case geometrize::ShapeTypes::CIRCLE:
        m_lastShape = std::make_shared<geometrize::Circle>(x, y, 64.0f);
        break;
    case geometrize::ShapeTypes::ELLIPSE:
        m_lastShape = std::make_shared<geometrize::Ellipse>(x, y, 64.0f, 32.0f);
        break;
    case geometrize::ShapeTypes::LINE:
        m_lastShape = std::make_shared<geometrize::Line>(x - 10, y - 10, x + 10, y + 10);
        break;
    case geometrize::ShapeTypes::POLYLINE:
        // TODO
        //m_lastShape = std::make_shared<geometrize::Polyline>(x - 10, y - 10, x + 10, y + 10);
        break;
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        // TODO
        break;
    case geometrize::ShapeTypes::RECTANGLE:
        m_lastShape = std::make_shared<geometrize::Rectangle>(x - 10, y - 10, x + 10, y + 10);
        break;
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        m_lastShape = std::make_shared<geometrize::RotatedEllipse>(x, y, 32.0f, 64.0f, 45.0f);
        break;
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        m_lastShape = std::make_shared<geometrize::RotatedRectangle>(x - 10, y - 10, x + 10, y + 10, 45.0f);
        break;
    case geometrize::ShapeTypes::TRIANGLE:
        m_lastShape = std::make_shared<geometrize::Triangle>(x - 10, y, x + 10, y, x, y - 10);
        break;
    default:
        assert(0 && "Unsupported shape type");
    }

    emit signal_didModifyShape(*m_lastShape.get());
}

void AreaOfInfluenceShape::setup(const geometrize::Shape& shape)
{
    m_lastShape = shape.clone();
    emit signal_didModifyShape(*m_lastShape.get());
}

void AreaOfInfluenceShape::translateShape(const float x, const float y)
{
    if(m_lastShape == nullptr) {
        assert(0 && "Last shape used was null, so there is no shape to translate");
        return;
    }
    geometrize::translate(*m_lastShape.get(), x, y);
    emit signal_didModifyShape(*m_lastShape.get());
}

void AreaOfInfluenceShape::scaleShape(const float scaleFactor)
{
    if(m_lastShape == nullptr) {
        assert(0 && "Last shape used was null, so there is no shape to scale");
        return;
    }

    geometrize::scale(*m_lastShape.get(), scaleFactor);
    emit signal_didModifyShape(*m_lastShape.get());
}

std::shared_ptr<geometrize::Shape> AreaOfInfluenceShape::getLastShape() const
{
    if(m_lastShape == nullptr) {
        return nullptr;
    }
    return m_lastShape->clone();
}

std::vector<std::pair<std::int32_t, std::int32_t>> AreaOfInfluenceShape::getPixels(const std::uint32_t xBound, const std::uint32_t yBound) const
{
    if(m_lastShape == nullptr) {
        return {};
    }

    const auto scanlines = geometrize::rasterize(*m_lastShape, static_cast<std::int32_t>(xBound), static_cast<std::int32_t>(yBound));

    std::vector<std::pair<std::int32_t, std::int32_t>> points = {};

    for(const auto& scanline : scanlines) {
        for(std::int32_t x = scanline.x1; x < scanline.x2; x++) {
            points.push_back({x, scanline.y});
        }
    }

    return points;
}

}

}
