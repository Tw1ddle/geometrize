#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobPixmapGraphicsItem class models a pixmap graphic item that goes into the scene for an image job.
 * This usually represents a pixmap of the working image that is being transformed into shapes.
 */
class ImageJobPixmapGraphicsItem : public QGraphicsPixmapItem
{
public:
    explicit ImageJobPixmapGraphicsItem();
    explicit ImageJobPixmapGraphicsItem(const QPixmap& pixmap);
    ~ImageJobPixmapGraphicsItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

}

}
