#include "imagejobpixmapscene.h"

#include <memory>

#include "dialog/imagejobpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

class ImageJobPixmapScene::ImageJobPixmapSceneImpl
{
public:
    ImageJobPixmapSceneImpl(ImageJobPixmapScene* pQ) : q{pQ}
    {
        q->addItem(&m_workingPixmapItem);
        q->addItem(&m_targetPixmapItem);
    }
    ImageJobPixmapSceneImpl operator=(const ImageJobPixmapSceneImpl&) = delete;
    ImageJobPixmapSceneImpl(const ImageJobPixmapSceneImpl&) = delete;
    ~ImageJobPixmapSceneImpl() = default;

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
    ImageJobPixmapScene* q;

    ImageJobPixmapGraphicsItem m_workingPixmapItem;
    ImageJobPixmapGraphicsItem m_targetPixmapItem;
};

ImageJobPixmapScene::ImageJobPixmapScene(QObject* parent) : QGraphicsScene{parent}, d{std::make_unique<ImageJobPixmapScene::ImageJobPixmapSceneImpl>(this)}
{
}

ImageJobPixmapScene::~ImageJobPixmapScene()
{
}

void ImageJobPixmapScene::setTargetPixmapOpacity(const float opacity)
{
    d->setTargetPixmapOpacity(opacity);
}

void ImageJobPixmapScene::setWorkingPixmap(const QPixmap& pixmap)
{
    d->setWorkingPixmap(pixmap);
}

void ImageJobPixmapScene::setTargetPixmap(const QPixmap& pixmap)
{
    d->setTargetPixmap(pixmap);
}

}

}
