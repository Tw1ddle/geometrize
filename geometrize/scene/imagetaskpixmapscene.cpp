#include "imagetaskpixmapscene.h"

#include <memory>

#include "scene/imagetaskpixmapgraphicsitem.h"

namespace geometrize
{

namespace scene
{

class ImageTaskPixmapScene::ImageTaskPixmapSceneImpl
{
public:
    ImageTaskPixmapSceneImpl(ImageTaskPixmapScene* pQ) : q{pQ}
    {
        q->addItem(&m_workingPixmapItem);
        m_workingPixmapItem.setZValue(-1);
    }
    ImageTaskPixmapSceneImpl operator=(const ImageTaskPixmapSceneImpl&) = delete;
    ImageTaskPixmapSceneImpl(const ImageTaskPixmapSceneImpl&) = delete;
    ~ImageTaskPixmapSceneImpl() = default;

    void setWorkingPixmap(const QPixmap& pixmap)
    {
        m_workingPixmapItem.setPixmap(pixmap);
    }

private:
    ImageTaskPixmapScene* q;
    ImageTaskPixmapGraphicsItem m_workingPixmapItem;
};

ImageTaskPixmapScene::ImageTaskPixmapScene(QObject* parent) : ImageTaskScene{parent}, d{std::make_unique<ImageTaskPixmapScene::ImageTaskPixmapSceneImpl>(this)}
{
}

ImageTaskPixmapScene::~ImageTaskPixmapScene()
{
}

void ImageTaskPixmapScene::setWorkingPixmap(const QPixmap& pixmap)
{
    d->setWorkingPixmap(pixmap);
}

}

}
