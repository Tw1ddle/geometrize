#include "imagetaskscene.h"

#include <memory>

#include <QFocusEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QString>

#include "scene/customtabletevent.h"
#include "scene/imagetaskpixmapgraphicsitem.h"
#include "scene/svgitem.h"

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
    }
    ImageTaskSceneImpl operator=(const ImageTaskSceneImpl&) = delete;
    ImageTaskSceneImpl(const ImageTaskSceneImpl&) = delete;
    ~ImageTaskSceneImpl() = default;

    void setTargetPixmapOpacity(float opacity)
    {
        // It's a pain to get completely transparent items to receive mouse/tablet events e.g. items()
        // However we need the target image to receive events, so we always make it slightly visible
        opacity = std::max(opacity, 0.001f);

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
            delete m_lastOverlayShapeItem;
        }
        if(item != nullptr) {
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
    // Handle custom tablet events forwarded from the ImageTaskGraphicsView
    if(event->type() == geometrize::scene::CustomTabletEvent::customEventId) {
        const auto ev = static_cast<geometrize::scene::CustomTabletEvent*>(event);
        QList<QGraphicsItem*> itemsUnderEvent = items(QPointF(ev->getData().xScenePos, ev->getData().yScenePos), Qt::IntersectsItemBoundingRect);
        for(const auto& item : itemsUnderEvent) {
            sendEvent(item, ev);
        }
        return true;
    }
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
