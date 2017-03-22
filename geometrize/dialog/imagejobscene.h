#pragma once

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
public:
    explicit ImageJobScene(QObject* parent = nullptr);
};

}

}
