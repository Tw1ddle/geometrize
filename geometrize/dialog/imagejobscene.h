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
 * @brief The ImageJobScene class models a graphics scene for viewing image jobs.
 */
class ImageJobScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit ImageJobScene(QObject* parent = nullptr);
    ~ImageJobScene();

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
    class ImageJobSceneImpl;
    std::unique_ptr<ImageJobSceneImpl> d;
};

}

}
