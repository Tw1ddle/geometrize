#include "imagejobpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

ImageJobPixmapGraphicsItem::ImageJobPixmapGraphicsItem() : QGraphicsPixmapItem()
{
    setFlag(ItemIsMovable, true);
}

ImageJobPixmapGraphicsItem::ImageJobPixmapGraphicsItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap)
{
}

ImageJobPixmapGraphicsItem::~ImageJobPixmapGraphicsItem()
{

}

void ImageJobPixmapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* /*event*/)
{

}

void ImageJobPixmapGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/)
{

}

void ImageJobPixmapGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* /*event*/)
{

}

}

}
