#include "imagetaskscene.h"

#include <memory>

#include <QFocusEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QString>

#include "scene/imagetaskpixmapgraphicsitem.h"
#include "scene/svgitem.h"
#include "scene/tools/areaofinfluenceshapes.h"

namespace geometrize
{

namespace scene
{

class ImageTaskScene::ImageTaskSceneImpl
{
public:
    ImageTaskSceneImpl(ImageTaskScene* pQ) : q{pQ}, m_lastOverlayShapeItem{nullptr}, m_overlayShapeVisibility{true}
    {
        q->setStickyFocus(true);

        q->addItem(&m_targetPixmapItem);
        m_targetPixmapItem.setZValue(1);
    }
    ImageTaskSceneImpl operator=(const ImageTaskSceneImpl&) = delete;
    ImageTaskSceneImpl(const ImageTaskSceneImpl&) = delete;
    ~ImageTaskSceneImpl() = default;

    void setTargetPixmapOpacity(const float opacity)
    {
        m_targetPixmapItem.setOpacity(static_cast<qreal>(opacity));
    }

    void setTargetPixmap(const QPixmap& pixmap)
    {
        m_targetPixmapItem.setPixmap(pixmap);
    }

    ImageTaskPixmapGraphicsItem& getTargetPixmapItem()
    {
        return m_targetPixmapItem;
    }

    void setOverlayShape(SvgItem* item)
    {
        if(m_lastOverlayShapeItem != nullptr) {
            q->removeItem(m_lastOverlayShapeItem);
        }
        if(item != nullptr) {
            item->setZValue(1000); // Should be on top of everything else
            item->setVisible(m_overlayShapeVisibility);
            q->addItem(item);
            q->setFocusItem(item, Qt::FocusReason::OtherFocusReason);
        }
        m_lastOverlayShapeItem = item;
    }

    void setOverlayShapeVisibility(const bool visible)
    {
        m_overlayShapeVisibility = visible;
        if(m_lastOverlayShapeItem != nullptr) {
            m_lastOverlayShapeItem->setVisible(m_overlayShapeVisibility);
        }
    }
	
protected:
    ImageTaskPixmapGraphicsItem m_targetPixmapItem;

private:
    ImageTaskScene* q;

    SvgItem* m_lastOverlayShapeItem;
    bool m_overlayShapeVisibility;
};

ImageTaskScene::ImageTaskScene(QObject* parent) : QGraphicsScene{parent}, d{std::make_unique<ImageTaskScene::ImageTaskSceneImpl>(this)}
{
}

ImageTaskScene::~ImageTaskScene()
{
}

void ImageTaskScene::setTargetPixmapOpacity(const float opacity)
{
    d->setTargetPixmapOpacity(opacity);
}

void ImageTaskScene::setTargetPixmap(const QPixmap& pixmap)
{
    d->setTargetPixmap(pixmap);
}

ImageTaskPixmapGraphicsItem& ImageTaskScene::getTargetPixmapItem()
{
    return d->getTargetPixmapItem();
}

void ImageTaskScene::setOverlayShape(SvgItem* item)
{
    d->setOverlayShape(item);
}

void ImageTaskScene::setOverlayShapeVisibility(const bool visible)
{
    d->setOverlayShapeVisibility(visible);
}

bool ImageTaskScene::event(QEvent* event)
{
    /*
    switch(event->type())
    {
        // Events forwarded from the ImageTaskGraphicsView
        case QEvent::TabletPress:
        case QEvent::TabletRelease:
        case QEvent::TabletMove:
        {
            // TODO also need to handle enter/exit proximity events with wacom tablet
            // TODO should also care about the device type probably
            const auto ev = static_cast<QTabletEvent*>(event); // TODO get tablet event + extras e.g. the global pos mapped to scene pos
            ev->
            //tabletEvent = new MyTabletEvent(ev);
            //sendEvent(itemAt(tabletEvent->scenePos()), tabletEvent);
            return true;
        }
        default:
            return QGraphicsScene::event(event);
    }
    */

    return QGraphicsScene::event(event);
}

void ImageTaskScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);
}

void ImageTaskScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void ImageTaskScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void ImageTaskScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    QGraphicsScene::wheelEvent(event);
}

void ImageTaskScene::keyPressEvent(QKeyEvent* event)
{
    QGraphicsScene::keyPressEvent(event);
}

void ImageTaskScene::keyReleaseEvent(QKeyEvent* event)
{
    QGraphicsScene::keyReleaseEvent(event);
}

void ImageTaskScene::focusInEvent(QFocusEvent* event)
{
    QGraphicsScene::focusInEvent(event);
}

void ImageTaskScene::focusOutEvent(QFocusEvent* event)
{
    QGraphicsScene::focusOutEvent(event);
}

}

}
