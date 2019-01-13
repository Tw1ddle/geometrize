#include "imagetaskscene.h"

#include <memory>

#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QString>

#include "scene/areaofinfluenceshape.h"
#include "scene/imagetaskpixmapgraphicsitem.h"
#include "scene/svgitem.h"

namespace geometrize
{

namespace scene
{

class ImageTaskScene::ImageTaskSceneImpl
{
public:
    ImageTaskSceneImpl(ImageTaskScene* pQ) : q{pQ}, m_lastOverlayShapeItem{nullptr}
    {
        q->addItem(&m_targetPixmapItem);
        m_targetPixmapItem.setZValue(1);
    }
    ImageTaskSceneImpl operator=(const ImageTaskSceneImpl&) = delete;
    ImageTaskSceneImpl(const ImageTaskSceneImpl&) = delete;
    ~ImageTaskSceneImpl() = default;

    void setTargetPixmapOpacity(const float opacity)
    {
        m_targetPixmapItem.setOpacity(opacity);
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
            q->addItem(item);
        }
        m_lastOverlayShapeItem = item;
    }
	
protected:
    ImageTaskPixmapGraphicsItem m_targetPixmapItem;

private:
    ImageTaskScene* q;

    SvgItem* m_lastOverlayShapeItem;
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

void ImageTaskScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    QGraphicsScene::wheelEvent(event);
}

}

}
