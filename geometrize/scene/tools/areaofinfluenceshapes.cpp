#include "areaofinfluenceshapes.h"

#include <cassert>
#include <utility>
#include <vector>

#include <Qt>

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

#include "scene/imagetaskscenemanager.h"

namespace
{

// Creates a set of default area of influence shapes
std::vector<std::shared_ptr<geometrize::Shape>> createDefaultShapes()
{
    std::vector<std::shared_ptr<geometrize::Shape>> shapes;

    // Centered on (0, 0), will need to translate to a sensible default position when used on canvas
    int x = 0;
    int y = 0;

    shapes.emplace_back(std::make_shared<geometrize::Circle>(x, y, 64.0f));
    shapes.emplace_back(std::make_shared<geometrize::Ellipse>(x, y, 64.0f, 32.0f));
    shapes.emplace_back(std::make_shared<geometrize::Line>(x - 10, y - 10, x + 10, y + 10));
    shapes.emplace_back(std::make_shared<geometrize::Rectangle>(x - 10, y - 10, x + 10, y + 10));
    shapes.emplace_back(std::make_shared<geometrize::RotatedEllipse>(x, y, 32.0f, 64.0f, 45.0f));
    shapes.emplace_back(std::make_shared<geometrize::RotatedRectangle>(x - 10, y - 10, x + 10, y + 10, 45.0f));
    shapes.emplace_back(std::make_shared<geometrize::Triangle>(x - 10, y, x + 10, y, x, y - 10));

    return shapes;
}

}

namespace geometrize
{

namespace scene
{

namespace tools
{

AreaOfInfluenceShapes::AreaOfInfluenceShapes() : m_currentShapeId(0)
{
    m_shapes = createDefaultShapes();
}

void AreaOfInfluenceShapes::connectToSceneManager(const geometrize::scene::ImageTaskSceneManager& mgr)
{
    // Connect pointer control/manipulations of the scenes to the actual attributes of influence shape
    connect(&mgr, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageMousePressEvent, [this](const double /*x*/, const double /*y*/, const bool ctrlModifier) {
        if(!ctrlModifier) {
            return;
        }
        selectNextShape();
    });
    connect(&mgr, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeMousePressEvent, [this](const double /*x*/, const double /*y*/, const bool ctrlModifier) {
        if(!ctrlModifier) {
            return;
        }
        selectNextShape();
    });
    connect(&mgr, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeHoverMoveEvent, [this](const int lastX, const int lastY, const int x, const int y, const bool ctrlModifier) {
        if(!ctrlModifier) {
            return;
        }
        translateShape(x - lastX, y - lastY);
    });
    connect(&mgr, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeMouseWheelEvent, [this](const int, const int, const double amount, const bool ctrlModifier) {
        if(!ctrlModifier) {
            return;
        }
        scaleShape(amount > 0 ? 1.03f : 0.97f);
    });
    connect(&mgr, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeKeyPressEvent, [this](const int key, const bool) {
        if(key == Qt::Key_R) { // Rotate
            rotateShape(3);
        }

        if(key == Qt::Key_Q) { // Scale down
            scaleShape(0.97f);
        }
        if(key == Qt::Key_A) { // Scale up
            scaleShape(1.03f);
        }
        if(key == Qt::Key::Key_Right) {
            selectNextShape();
        }
    });
}

void AreaOfInfluenceShapes::setup(const geometrize::Shape& shape)
{
    m_shapes[m_currentShapeId] = shape.clone();
    emit signal_didModifyShape(*getCurrentShape()->clone().get());
}

void AreaOfInfluenceShapes::selectNextShape()
{
    m_currentShapeId++;
    if(m_currentShapeId >= m_shapes.size()) {
        m_currentShapeId = 0;
    }
    emit signal_didModifyShape(*getCurrentShape()->clone().get());
}

void AreaOfInfluenceShapes::translateShape(const float x, const float y)
{
    if(getCurrentShape() == nullptr) {
        assert(0 && "Last shape used was null, so there is no shape to translate");
        return;
    }
    geometrize::translate(*getCurrentShape().get(), x, y);
    emit signal_didModifyShape(*getCurrentShape()->clone().get());
}

void AreaOfInfluenceShapes::rotateShape(const float angle)
{
    if(getCurrentShape() == nullptr) {
        assert(0 && "Last shape used was null, so there is no shape to rotate");
        return;
    }

    geometrize::rotate(*getCurrentShape().get(), angle);
    emit signal_didModifyShape(*getCurrentShape()->clone().get());
}

void AreaOfInfluenceShapes::scaleShape(const float scaleFactor)
{
    if(getCurrentShape() == nullptr) {
        assert(0 && "Last shape used was null, so there is no shape to scale");
        return;
    }

    geometrize::scale(*getCurrentShape().get(), scaleFactor);
    emit signal_didModifyShape(*getCurrentShape()->clone().get());
}

std::shared_ptr<geometrize::Shape> AreaOfInfluenceShapes::getCurrentShape() const
{
    if(m_shapes.empty()) {
        return nullptr;
    }
    return m_shapes.at(m_currentShapeId);
}

std::vector<std::pair<std::int32_t, std::int32_t>> AreaOfInfluenceShapes::getPixels(const std::uint32_t xBound, const std::uint32_t yBound) const
{
    if(getCurrentShape() == nullptr) {
        return {};
    }

    const auto scanlines = geometrize::rasterize(*getCurrentShape(), static_cast<std::int32_t>(xBound), static_cast<std::int32_t>(yBound));

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

}
