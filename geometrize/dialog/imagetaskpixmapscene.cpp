#include "imagetaskpixmapscene.h"

#include <memory>

#include "dialog/imagetaskpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

class ImageTaskPixmapScene::ImageTaskPixmapSceneImpl
{
public:
    ImageTaskPixmapSceneImpl(ImageTaskPixmapScene* pQ) : q{pQ}
    {
        q->addItem(&m_workingPixmapItem);
        q->addItem(&m_targetPixmapItem);
    }
    ImageTaskPixmapSceneImpl operator=(const ImageTaskPixmapSceneImpl&) = delete;
    ImageTaskPixmapSceneImpl(const ImageTaskPixmapSceneImpl&) = delete;
    ~ImageTaskPixmapSceneImpl() = default;

    void setTargetPixmapOpacity(const float opacity)
    {
        m_targetPixmapItem.setOpacity(opacity);
    }

    void setWorkingPixmap(const QPixmap& pixmap)
    {
        m_workingPixmapItem.setPixmap(pixmap);
    }

    void setTargetPixmap(const QPixmap& pixmap)
    {
        m_targetPixmapItem.setPixmap(pixmap);
    }

private:
    ImageTaskPixmapScene* q;

    ImageTaskPixmapGraphicsItem m_workingPixmapItem;
    ImageTaskPixmapGraphicsItem m_targetPixmapItem;
};

ImageTaskPixmapScene::ImageTaskPixmapScene(QObject* parent) : QGraphicsScene{parent}, d{std::make_unique<ImageTaskPixmapScene::ImageTaskPixmapSceneImpl>(this)}
{
}

ImageTaskPixmapScene::~ImageTaskPixmapScene()
{
}

void ImageTaskPixmapScene::setTargetPixmapOpacity(const float opacity)
{
    d->setTargetPixmapOpacity(opacity);
}

void ImageTaskPixmapScene::setWorkingPixmap(const QPixmap& pixmap)
{
    d->setWorkingPixmap(pixmap);
}

void ImageTaskPixmapScene::setTargetPixmap(const QPixmap& pixmap)
{
    d->setTargetPixmap(pixmap);
}

}

}
