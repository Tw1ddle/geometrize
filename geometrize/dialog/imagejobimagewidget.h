#pragma once

#include <memory>

#include <QWidget>

class QImage;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobImageWidget implements a widget for viewing and manipulating the images that an image job works with.
 */
class ImageJobImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageJobImageWidget(QWidget* parent = nullptr);
    ~ImageJobImageWidget();

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

    /**
     * @brief baseImageSelected Signal dispatched when a candidate base image is selected, typically by the user.
     * @param image The image the user picked.
     */
    void baseImageSelected(const QImage& image);

private:
    class ImageJobImageWidgetImpl;
    std::unique_ptr<ImageJobImageWidgetImpl> d;
};

}

}
