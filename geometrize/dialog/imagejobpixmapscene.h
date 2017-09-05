#pragma once

#include <memory>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobPixmapScene class models a graphics scene for viewing the pixmap-based results of image jobs.
 */
class ImageJobPixmapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit ImageJobPixmapScene(QObject* parent = nullptr);
    ~ImageJobPixmapScene();

    /**
     * @brief setTargetPixmapOpacity Sets the opacity of the target pixmap.
     * @param opacity The opacity value.
     */
    void setTargetPixmapOpacity(float opacity);

    /**
     * @brief setWorkingPixmap Sets the pixmap that provides the current/working image visualization.
     * @param pixmap The pixmap to set as the current/working image visualization.
     */
    void setWorkingPixmap(const QPixmap& pixmap);

    /**
     * @brief setTargetPixmap Sets the pixmap that provides the target/goal image visualization.
     * @param pixmap The pixmap to set as the target/goal image visualization.
     */
    void setTargetPixmap(const QPixmap& pixmap);

private:
    class ImageJobPixmapSceneImpl;
    std::unique_ptr<ImageJobPixmapSceneImpl> d;
};

}

}
