#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <QWidget>

class QEvent;

namespace geometrize
{

namespace task
{
class ImageTask;
}

}

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
 * @brief The ImageTaskScriptingWidget implements a widget for manipulating scripts that are run when an image is being geometrized e.g. before/after a shape is added.
 */
class ImageTaskScriptingWidget : public QWidget
{
    Q_OBJECT

public:
    ImageTaskScriptingWidget(QWidget* parent = nullptr);
    virtual ~ImageTaskScriptingWidget();

    /**
     * @brief setImageTask Sets the current image task used by the scripts.
     * @param task Non-owning pointer to the image task that the scripts on this widget will use.
     */
    void setImageTask(task::ImageTask* task);

    /**
     * @brief syncUserInterface Syncs the user interface with the current image task.
     * This should be called after setting a new image task, or new task settings.
     */
    void syncUserInterface();

    std::map<std::string, std::string> getShapeMutationScripts() const;

    /**
     * @brief evaluateBeforeStepScripts Evaluates the scripts set to run directly before the image task is stepped (i.e. immediately before the image task starts a step).
     */
    void evaluateBeforeStepScripts() const;

    /**
     * @brief evaluateAfterStepScripts Evaluates the scripts set to run directly after the image task is stepped (i.e. immediately after the image task starts a step).
     */
    void evaluateAfterStepScripts() const;

    /**
     * @brief evaluateStopConditionScripts Returns true if the condition(s) for stopping geometrizing the image were met. This is run each time the image task completes a step.
     * @return True if a stop condition was met
     */
    bool evaluateStopConditionScripts() const;

    /**
     * @brief evaluateBeforeAddShapeScripts Evaluates the scripts set to run directly before a shape is added to the image.
     */
    void evaluateBeforeAddShapeScripts() const;

    /**
     * @brief evaluateAfterAddShapeScripts Evaluates the scripts set to run directly after a shape is added to the image.
     */
    void evaluateAfterAddShapeScripts() const;

    /**
     * @brief evaluateOnPenInputEventScripts Evaluates the scripts set to run directly after a pen e.g. Wacom stylus interacts with the scenes
     */
    void evaluateOnPenInputEventScripts() const;

    /**
     * @brief evaluateOnPenProximityEnterEventScripts Evaluates the scripts set to run directly after a pen e.g. Wacom stylus enters proximity of the screen (application global, not just the scenes)
     */
    void evaluateOnPenProximityEnterEventScripts() const;

    /**
     * @brief evaluateOnPenProximityExitEventScripts Evaluates the scripts set to run directly after a pen e.g. Wacom stylus leaves the proximity of the screen (application global, not just the scenes)
     */
    void evaluateOnPenProximityExitEventScripts() const;

signals:
    void signal_scriptChanged(const std::string& functionName, const std::string& code);

protected:
    void changeEvent(QEvent*) override;

private:
    class ImageTaskScriptingWidgetImpl;
    std::unique_ptr<ImageTaskScriptingWidgetImpl> d;
};

}

}
