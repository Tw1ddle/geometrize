#include "svgitem.h"

#include <QByteArray>
#include <QKeyEvent>
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

    setAcceptHoverEvents(true); // Listening for hover events on the pixmap target image for now
    setFlag(ItemIsMovable, false);
}

SvgItem::~SvgItem()
{
	delete renderer();
}

void SvgItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsSvgItem::hoverMoveEvent(event);
    emit signal_onHoverMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(), event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsSvgItem::mousePressEvent(event);
    emit signal_onMousePressEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsSvgItem::mouseReleaseEvent(event);
    emit signal_onMouseReleaseEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsSvgItem::mouseMoveEvent(event);
    emit signal_onMouseMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(), event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    QGraphicsSvgItem::wheelEvent(event);
    emit signal_onWheelEvent(event->scenePos().x(), event->scenePos().y(), event->delta(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::keyPressEvent(QKeyEvent* event)
{
    QGraphicsSvgItem::keyPressEvent(event);
    emit signal_onKeyPressEvent(event->key(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::keyReleaseEvent(QKeyEvent* event)
{
    QGraphicsSvgItem::keyReleaseEvent(event);
    emit signal_onKeyReleaseEvent(event->key(), event->modifiers() & Qt::ControlModifier);
}

void SvgItem::focusInEvent(QFocusEvent* event)
{
    QGraphicsSvgItem::focusInEvent(event);
    emit signal_onFocusInEvent();
}

void SvgItem::focusOutEvent(QFocusEvent* event)
{
    QGraphicsSvgItem::focusOutEvent(event);
    emit signal_onFocusOutEvent();
}

}

}
