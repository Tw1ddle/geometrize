#include "imagetaskpixmapgraphicsitem.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QPixmap>

namespace geometrize
{

namespace scene
{

ImageTaskPixmapGraphicsItem::ImageTaskPixmapGraphicsItem() : QGraphicsPixmapItem()
{
    setAcceptHoverEvents(true);
    setFlag(ItemIsMovable, false);
}

ImageTaskPixmapGraphicsItem::ImageTaskPixmapGraphicsItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap)
{
}

ImageTaskPixmapGraphicsItem::~ImageTaskPixmapGraphicsItem()
{
}

void ImageTaskPixmapGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsPixmapItem::hoverMoveEvent(event);
    emit signal_onHoverMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(), event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    emit signal_onMouseMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(),event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    emit signal_onMouseReleaseEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
    emit signal_onMouseMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(), event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    QGraphicsPixmapItem::wheelEvent(event);
    emit signal_onWheelEvent(event->scenePos().x(), event->scenePos().y(), event->delta(), event->modifiers() & Qt::ControlModifier);
}

}

}
