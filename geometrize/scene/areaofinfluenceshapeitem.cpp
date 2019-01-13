#include "areaofinfluenceshapeitem.h"

#include <QByteArray>
#include <QGraphicsSceneWheelEvent>

namespace geometrize
{

namespace scene
{

AreaOfInfluenceShapeItem::AreaOfInfluenceShapeItem(const QByteArray& data) : SvgItem{data}
{
}

AreaOfInfluenceShapeItem::~AreaOfInfluenceShapeItem()
{
}

void AreaOfInfluenceShapeItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    // Would be nice to only respond to this if we are moused over a section
    // of the shape we can see, rather than the whole SVG item - but Qt doesn't seem to support this
    SvgItem::wheelEvent(event);
}

}

}
