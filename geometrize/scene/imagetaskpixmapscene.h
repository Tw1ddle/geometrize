#pragma once

#include <memory>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>

#include "imagetaskscene.h"

namespace geometrize
{

namespace scene
{

/**
 * @brief The ImageTaskPixmapScene class models a graphics scene for viewing the pixmap-based results of image tasks.
 */
class ImageTaskPixmapScene : public ImageTaskScene
{
    Q_OBJECT

public:
    explicit ImageTaskPixmapScene(QObject* parent = nullptr);
    virtual ~ImageTaskPixmapScene();

    /**
     * @brief setWorkingPixmap Sets the pixmap that provides the current/working image visualization.
     * @param pixmap The pixmap to set as the current/working image visualization.
     */
    void setWorkingPixmap(const QPixmap& pixmap);

private:
    class ImageTaskPixmapSceneImpl;
    std::unique_ptr<ImageTaskPixmapSceneImpl> d;
};

}

}
