#include "imagetaskgraphicsview.h"

#include <QEvent>
#include <QWheelEvent>

namespace geometrize
{

namespace dialog
{

ImageTaskGraphicsView::ImageTaskGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setMouseTracking(true);

    populateUi();
}

void ImageTaskGraphicsView::wheelEvent(QWheelEvent* e)
{
    if (e->angleDelta().x() == 0) {
        const QPoint pos{e->pos()};
        const QPointF posf{mapToScene(pos)};

        const double angle{static_cast<double>(e->angleDelta().y())};
        double by{0.0};
        if(angle > 0) {
            by = 1 + (angle / 360 * 0.1);
        } else if (angle < 0) {
            by = 1 - (-angle / 360 * 0.1);
        } else {
            by = 1;
        }

        scale(by, by);

        const int w{viewport()->width()};
        const int h{viewport()->height()};
        const double wf{mapToScene(QPoint(w - 1, 0)).x() -mapToScene(QPoint(0,0)).x()};
        const double hf{mapToScene(QPoint(0, h - 1)).y() - mapToScene(QPoint(0,0)).y()};
        const double lf{posf.x() - pos.x() * wf / w};
        const double tf{posf.y() - pos.y() * hf / h};

        ensureVisible(lf, tf, wf, hf, 0, 0);
        const QPointF newPos{mapToScene(pos)};
        ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf, QSizeF(wf, hf)), 0, 0);

        e->accept();
    }
}

void ImageTaskGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void ImageTaskGraphicsView::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        populateUi();
    }
    QGraphicsView::changeEvent(event);
}

void ImageTaskGraphicsView::populateUi()
{

}

}

}
