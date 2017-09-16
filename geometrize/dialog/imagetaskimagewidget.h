#pragma once

#include <memory>

#include <QWidget>

class QEvent;
class QImage;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskImageWidget implements a widget for viewing and manipulating the images that an image task works with.
 */
class ImageTaskImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageTaskImageWidget(QWidget* parent = nullptr);
    ~ImageTaskImageWidget();

    /**
     * @brief setTargetImageOpacity Sets the opacity of the target image.
     * @param opacity The opacity of the target image (0-255).
     */
    void setTargetImageOpacity(unsigned int opacity);

    /**
     * @brief setTargetImage Sets the target image, which is made visible in this widget. Must be a valid (non-null) image.
     * @param image The target image to set on this widget.
     */
    void setTargetImage(const QImage& image);

signals:
    /**
     * @brief targetImageOpacityChanged Signal dispatched when the target image opacity is changed.
     * @param opacity The opacity of the target image (0-255)
     */
    void targetImageOpacityChanged(unsigned int opacity);

    /**
     * @brief targetImageSelected Signal dispatched when a candidate target image is selected, typically by the user.
     * @param image The image the user picked.
     */
    void targetImageSelected(const QImage& image);

    /**
     * @brief targetImageSet Signal dispatched when a target image is set and made visible in this widget (and is probably going to be used by the image task).
     * @param image The image that was set on the target image widget.
     */
    void targetImageSet(const QImage& image);

    /**
     * @brief baseImageSelected Signal dispatched when a candidate base image is selected, typically by the user.
     * @param image The image the user picked.
     */
    void baseImageSelected(const QImage& image);

protected:
    void changeEvent(QEvent*) override;

private:
    class ImageTaskImageWidgetImpl;
    std::unique_ptr<ImageTaskImageWidgetImpl> d;
};

}

}
