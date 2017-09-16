#include "imagetaskpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

ImageTaskPixmapGraphicsItem::ImageTaskPixmapGraphicsItem() : QGraphicsPixmapItem()
{
    setFlag(ItemIsMovable, false);
}

ImageTaskPixmapGraphicsItem::ImageTaskPixmapGraphicsItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap)
{
}

ImageTaskPixmapGraphicsItem::~ImageTaskPixmapGraphicsItem()
{
}

void ImageTaskPixmapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
}

void ImageTaskPixmapGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void ImageTaskPixmapGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

}

}
