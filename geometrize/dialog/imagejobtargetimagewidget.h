#pragma once

#include <memory>

#include <QWidget>

class QImage;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobTargetImageWidget implements a widget for viewing and manipulating the image that an image job reproduces using shapes.
 */
class ImageJobTargetImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageJobTargetImageWidget(QWidget* parent = nullptr);
    ~ImageJobTargetImageWidget();

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
     * @brief targetImageSet Signal dispatched when a target image is set and made visible in this widget (and is probably going to be used by the image job).
     * @param image The image that was set on the target image widget.
     */
    void targetImageSet(const QImage& image);

private:
    class ImageJobTargetImageWidgetImpl;
    std::unique_ptr<ImageJobTargetImageWidgetImpl> d;
};

}

}
