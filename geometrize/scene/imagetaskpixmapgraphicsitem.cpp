#include "imagetaskpixmapgraphicsitem.h"

#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QPixmap>

#include "customtabletevent.h"

namespace geometrize
{

namespace scene
{

ImageTaskPixmapGraphicsItem::ImageTaskPixmapGraphicsItem() : QGraphicsPixmapItem()
{
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setFlag(ItemIsMovable, false);
    setFlag(ItemIsSelectable, false);
    setFlag(ItemIsFocusable, true); // To receive key events (need focus first of course)
    setZValue(1000);

    // Allows the pixmap to receive events even when the opacity is set to 0
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

ImageTaskPixmapGraphicsItem::ImageTaskPixmapGraphicsItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap)
{
}

ImageTaskPixmapGraphicsItem::~ImageTaskPixmapGraphicsItem()
{
}

bool ImageTaskPixmapGraphicsItem::sceneEvent(QEvent* event)
{
    // Handle custom tablet events forwarded from the ImageTaskScene
    if(event->type() == geometrize::scene::CustomTabletEvent::customEventId) {
        emit signal_onCustomTabletEvent(*static_cast<geometrize::scene::CustomTabletEvent*>(event));
        return true;
    }
    return QGraphicsPixmapItem::sceneEvent(event);
}

void ImageTaskPixmapGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    // This is done so the parent scene/view still gets these events, so long as control isn't pressed
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::hoverMoveEvent(event);
    emit signal_onHoverMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(), event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::mousePressEvent(event);
    emit signal_onMousePressEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::mouseReleaseEvent(event);
    emit signal_onMouseReleaseEvent(event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::mouseMoveEvent(event);
    emit signal_onMouseMoveEvent(event->lastScenePos().x(), event->lastScenePos().y(), event->scenePos().x(), event->scenePos().y(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::wheelEvent(event);
    emit signal_onWheelEvent(event->scenePos().x(), event->scenePos().y(), event->delta(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::keyPressEvent(QKeyEvent* event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::keyPressEvent(event);
    emit signal_onKeyPressEvent(event->key(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        event->setAccepted(false);
    }

    QGraphicsPixmapItem::keyReleaseEvent(event);
    emit signal_onKeyReleaseEvent(event->key(), event->modifiers() & Qt::ControlModifier);
}

void ImageTaskPixmapGraphicsItem::focusInEvent(QFocusEvent* event)
{
    QGraphicsPixmapItem::focusInEvent(event);
    emit signal_onFocusInEvent();
}

void ImageTaskPixmapGraphicsItem::focusOutEvent(QFocusEvent* event)
{
    QGraphicsPixmapItem::focusOutEvent(event);
    emit signal_onFocusOutEvent();
}

}

}
