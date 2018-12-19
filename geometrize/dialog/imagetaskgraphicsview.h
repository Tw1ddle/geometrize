#pragma once

#include <QGraphicsView>
#include <QObject>

class QEvent;
class QWheelEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskGraphicsView class models a graphics view for viewing the images and shapes used and/or produced by image tasks.
 */
class ImageTaskGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageTaskGraphicsView(QWidget* parent = nullptr);
    virtual ~ImageTaskGraphicsView() = default;

protected:
    void changeEvent(QEvent*) override;

private:
    void populateUi();

    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

}

}
