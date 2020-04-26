#pragma once

#include <QGraphicsView>
#include <QObject>

class QEvent;
class QKeyEvent;
class QWheelEvent;

namespace geometrize
{

namespace scene
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

    bool viewportEvent(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

}

}
