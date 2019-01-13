#include "svgitem.h"

#include <QByteArray>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

namespace geometrize
{

namespace scene
{

SvgItem::SvgItem(const QByteArray& data) : QGraphicsSvgItem{}
{
	setSharedRenderer(new QSvgRenderer(data));

    setAcceptHoverEvents(false); // Listening for hover events on the pixmap target image for now
    setFlag(ItemIsMovable, false);
}

SvgItem::~SvgItem()
{
	delete renderer();
}

void SvgItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsSvgItem::hoverMoveEvent(event);
    emit signal_onHoverMoveEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsSvgItem::mousePressEvent(event);
    emit signal_onMouseMoveEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsSvgItem::mouseReleaseEvent(event);
    emit signal_onMouseReleaseEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsSvgItem::mouseMoveEvent(event);
    emit signal_onMouseMoveEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    QGraphicsSvgItem::wheelEvent(event);
    emit signal_onWheelEvent(event->scenePos().x(), event->scenePos().y(), event->delta(), event->modifiers() & Qt::ControlModifier);
}

}

}
