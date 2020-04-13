#pragma once

#include <cstdint>
#include <memory>

#include <QWidget>

class QEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief showImageTaskStopConditionMetMessage Shows a messagebox stating that the stop conditions for geometrizing an image were met
 * @param parent The parent widget to center the message box on
 */
void showImageTaskStopConditionMetMessage(QWidget* parent);

/**
 * @brief The ImageTaskPrePostScriptsWidget implements a widget for writing scripts that are repeatedly run while an image is being geometrized, usually before/after a shape is added.
 */
class ImageTaskPrePostScriptsWidget : public QWidget
{
    Q_OBJECT

public:
    ImageTaskPrePostScriptsWidget(QWidget* parent = nullptr);
    virtual ~ImageTaskPrePostScriptsWidget();

    /**
     * @brief addStopCondition Adds a stop condition script that will be evaluated when stopConditionsMet is called
     * @param scriptCode The ChaiScript code for checking the stop condition (must evaluate to a boolean returning true to stop)
     */
    void addStopCondition(const std::string& scriptCode);

    /**
     * @brief stopConditionsMet Returns true if the condition(s) for stopping geometrizing the image were met
     * @param currentShapeCount The number of shapes that have been geometrized so far
     * @return True if a stop condition was met
     */
    bool stopConditionsMet(std::size_t currentShapeCount) const;

protected:
    void changeEvent(QEvent*) override;

private:
    class ImageTaskPrePostScriptsWidgetImpl;
    std::unique_ptr<ImageTaskPrePostScriptsWidgetImpl> d;
};

}

}
