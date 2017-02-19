#pragma once

#include <QGraphicsView>
#include <QObject>

class QWheelEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageGraphicsView class models a graphics view for viewing image jobs.
 */
class ImageGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageGraphicsView(QWidget* parent = nullptr);

private:
    void wheelEvent(QWheelEvent* event) override;
};

}

}
