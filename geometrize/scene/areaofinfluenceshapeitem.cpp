#include "areaofinfluenceshapeitem.h"

#include <QByteArray>
#include <QGraphicsSceneWheelEvent>

namespace geometrize
{

namespace scene
{

AreaOfInfluenceShapeItem::AreaOfInfluenceShapeItem(const QByteArray& data) : SvgItem{data}
{
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setFlag(ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setCacheMode(QGraphicsItem::NoCache);
    setZValue(1000);
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

void AreaOfInfluenceShapeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    SvgItem::hoverMoveEvent(event);
}

void AreaOfInfluenceShapeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    SvgItem::mousePressEvent(event);
}

void AreaOfInfluenceShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    SvgItem::mouseReleaseEvent(event);
}

void AreaOfInfluenceShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    SvgItem::mouseMoveEvent(event);
}

void AreaOfInfluenceShapeItem::keyPressEvent(QKeyEvent* event)
{
    SvgItem::keyPressEvent(event);
}

void AreaOfInfluenceShapeItem::keyReleaseEvent(QKeyEvent* event)
{
    SvgItem::keyReleaseEvent(event);
}

void AreaOfInfluenceShapeItem::focusInEvent(QFocusEvent* event)
{
    SvgItem::focusInEvent(event);
}

void AreaOfInfluenceShapeItem::focusOutEvent(QFocusEvent* event)
{
    SvgItem::focusOutEvent(event);
}

}

}
