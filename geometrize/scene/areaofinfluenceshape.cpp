#include "areaofinfluenceshape.h"

#include "geometrize/shape/circle.h"

#include <cassert>

namespace geometrize
{

namespace scene
{

void AreaOfInfluenceShape::setup(const int x, const int y, const unsigned int radius)
{
    // Just create an entire new shape every time (since that isn't very expensive)
    m_lastShape = std::make_shared<geometrize::Circle>(x, y, static_cast<int>(radius));
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
    m_lastShape->translate(x, y);
    emit signal_didModifyShape(*m_lastShape.get());
}

void AreaOfInfluenceShape::scaleShape(const float scaleFactor)
{
    if(m_lastShape == nullptr) {
        assert(0 && "Last shape used was null, so there is no shape to scale");
        return;
    }

    m_lastShape->scale(scaleFactor);
    emit signal_didModifyShape(*m_lastShape.get());
}

std::shared_ptr<geometrize::Shape> AreaOfInfluenceShape::getLastShape() const
{
    if(m_lastShape == nullptr) {
        return nullptr;
    }
    return m_lastShape->clone();
}

}

}
