#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskPixmapGraphicsItem class models a pixmap graphic item that goes into the scene for an image task.
 * This usually represents a pixmap of the working image that is being transformed into shapes.
 */
class ImageTaskPixmapGraphicsItem : public QGraphicsPixmapItem
{
public:
    explicit ImageTaskPixmapGraphicsItem();
    explicit ImageTaskPixmapGraphicsItem(const QPixmap& pixmap);
    virtual ~ImageTaskPixmapGraphicsItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

}

}
