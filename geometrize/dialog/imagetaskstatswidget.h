#pragma once

#include <memory>

#include <QWidget>

class QEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskStatsWidget implements a widget for statistics relating to image task progress, such as the number of shapes created so far.
 */
class ImageTaskStatsWidget : public QWidget
{
    Q_OBJECT

public:
    enum ImageTaskStatus {
        STOPPED,
        RUNNING
    };

    ImageTaskStatsWidget(QWidget* parent = nullptr);
    virtual ~ImageTaskStatsWidget();

    void setTaskId(std::size_t id);
    void setTimeRunning(int seconds);
    void setCurrentStatus(ImageTaskStatus status);
    void setShapeCount(std::size_t shapeCount);
    void setSimilarity(float similarity);
    void setImageDimensions(std::uint32_t width, std::uint32_t height);

protected:
    void changeEvent(QEvent*) override;

private:
    class ImageTaskStatsWidgetImpl;
    std::unique_ptr<ImageTaskStatsWidgetImpl> d;
};

}

}
