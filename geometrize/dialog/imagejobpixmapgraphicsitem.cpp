#include "imagejobpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

ImageJobPixmapGraphicsItem::ImageJobPixmapGraphicsItem() : QGraphicsPixmapItem()
{
    setFlag(ItemIsMovable, false);
}

ImageJobPixmapGraphicsItem::ImageJobPixmapGraphicsItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap)
{
}

ImageJobPixmapGraphicsItem::~ImageJobPixmapGraphicsItem()
{
}

void ImageJobPixmapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
}

void ImageJobPixmapGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void ImageJobPixmapGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

}

}
