#include "shapecollection.h"

#include <vector>

#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace task
{

bool ShapeCollection::empty() const
{
    return m_shapes.empty();
}

std::size_t ShapeCollection::size() const
{
    return m_shapes.size();
}

void ShapeCollection::clear()
{
    m_shapes.clear();
    emit signal_sizeChanged(m_shapes.size());
}

const std::vector<geometrize::ShapeResult>& ShapeCollection::getShapeVector() const
{
    return m_shapes;
}

void ShapeCollection::appendShapes(const std::vector<geometrize::ShapeResult>& shapes)
{
    emit signal_beforeAppendShapes(shapes);
    std::copy(shapes.begin(), shapes.end(), std::back_inserter(m_shapes));
    emit signal_sizeChanged(m_shapes.size());
    emit signal_afterAppendShapes(shapes);
}

geometrize::ShapeResult& ShapeCollection::back()
{
    return m_shapes.back();
}

}
}
