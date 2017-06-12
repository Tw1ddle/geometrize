#pragma once

#include <memory>

#include <QGraphicsView>
#include <QObject>

class QWheelEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The SvgPreviewView class provides a graphics view for viewing SVGs.
 */
class SvgPreviewView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit SvgPreviewView(QWidget* parent = nullptr);
    ~SvgPreviewView();

private:
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    class SvgPreviewViewImpl;
    std::unique_ptr<SvgPreviewViewImpl> d;
};

}

}
