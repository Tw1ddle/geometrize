#include "imagejobscene.h"

#include <memory>

#include "dialog/imagejobpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

class ImageJobScene::ImageJobSceneImpl
{
public:
    ImageJobSceneImpl(ImageJobScene* pQ) : q{pQ}
    {
        q->addItem(&m_workingPixmapItem);
        q->addItem(&m_targetPixmapItem);
    }
    ImageJobSceneImpl operator=(const ImageJobSceneImpl&) = delete;
    ImageJobSceneImpl(const ImageJobSceneImpl&) = delete;
    ~ImageJobSceneImpl() = default;

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
    ImageJobScene* q;

    ImageJobPixmapGraphicsItem m_workingPixmapItem;
    ImageJobPixmapGraphicsItem m_targetPixmapItem;
};

ImageJobScene::ImageJobScene(QObject* parent) : QGraphicsScene{parent}, d{std::make_unique<ImageJobScene::ImageJobSceneImpl>(this)}
{
}

ImageJobScene::~ImageJobScene()
{
}

void ImageJobScene::setTargetPixmapOpacity(const float opacity)
{
    d->setTargetPixmapOpacity(opacity);
}

void ImageJobScene::setWorkingPixmap(const QPixmap& pixmap)
{
    d->setWorkingPixmap(pixmap);
}

void ImageJobScene::setTargetPixmap(const QPixmap& pixmap)
{
    d->setTargetPixmap(pixmap);
}

}

}
