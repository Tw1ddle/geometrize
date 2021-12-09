#pragma once

#include <map>
#include <string>
#include <vector>

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace script
{

/**
 * @brief sleep Causes the calling thread to sleep for the given length of time.
 * @param ms The number of milliseconds to sleep for.
 */
void sleep(std::size_t ms);

/**
 * @brief getEngineFunctionNames Gets the names of the functions registered with the given engine.
 * @param engine The engine whose function names will be fetched.
 * @return The names registered with the given engine.
 */
std::vector<std::string> getEngineFunctionNames(const chaiscript::ChaiScript& engine);

/**
 * @brief getDefaultScripts Gets a map of the scripts in the given folder.
 * @param folder Path to the folder containing the ChaiScript scripts to retrieve. The files must have the ".chai" file extension.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getScripts(const std::string& folder);

/**
 * @brief getDefaultScripts Gets a map of the default scripts used for shape creation and mutation in image tasks.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getDefaultShapeMutatorScripts();

/**
 * @brief getPointerAreaOfInterestScripts Gets a map of the scripts used for shape creation and mutation based around user input/pointer interaction.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getPointerAreaOfInterestShapeMutatorScripts();

/**
 * @brief getBeforeStepCallbackScripts Gets a map of scripts that are useful to call prior to stepping an image task (i.e. before starting work to find a shape to add).
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getBeforeStepCallbackScripts();

/**
 * @brief getAfterStepCallbackScripts Gets a map of scripts that are useful to call after stepping an image task (i.e. after completing work to find a shape to add).
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getAfterStepCallbackScripts();

/**
 * @brief getStopConditionScripts Gets a map of scripts that get to true/false, and can be used to check whether to stop an image task (usually after stepping).
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getStopConditionScripts();

/**
 * @brief getOnStopConditionMetScripts Gets a map of scripts that are useful to call after the stop condition for an image task is met.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnStopConditionMetScripts();

/**
 * @brief getAddShapePreconditionScripts Gets a map of scripts that are used to decide whether to add a shape to an image or not
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getAddShapePreconditionScripts();

/**
 * @brief getBeforeAddShapeCallbackScripts Gets a map of scripts that are useful to call before adding a shape to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getBeforeAddShapeCallbackScripts();

/**
 * @brief getAfterAddShapeCallbackScripts Gets a map of scripts that are useful to call after adding a shape to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getAfterAddShapeCallbackScripts();

/**
 * @brief getOnPenInputCallbackScripts Gets a map of scripts that are useful to call after receiving pen input to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnPenInputCallbackScripts();

/**
 * @brief getOnPenProximityEnterCallbackScripts Gets a map of scripts that are useful to call after receiving a pen proximity enter event. This is application global, not bound to a particular view.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnPenProximityEnterCallbackScripts();

/**
 * @brief getOnPenProximityExitCallbackScripts Gets a map of scripts that are useful to call after receiving a pen proximity exit event. This is application global, not bound to a particular view.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnPenProximityExitCallbackScripts();

/**
 * @brief getOnKeyDownEventScripts Gets a map of scripts that are useful to call after receiving a key down event to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnKeyDownEventScripts();

/**
 * @brief getOnKeyUpEventScripts Gets a map of scripts that are useful to call after receiving a key up event to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnKeyUpEventScripts();

/**
 * @brief getOnMouseDownEventScripts Gets a map of scripts that are useful to call after receiving a mouse down event to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnMouseDownEventScripts();

/**
 * @brief getOnMouseUpEventScripts Gets a map of scripts that are useful to call after receiving a mouse up event to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnMouseUpEventScripts();

/**
 * @brief getOnMouseMoveEventScripts Gets a map of scripts that are useful to call after receiving a mouse move event to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnMouseMoveEventScripts();

/**
 * @brief getOnMouseWheelEventScripts Gets a map of scripts that are useful to call after receiving a mouse wheel event to the image task views.
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnMouseWheelEventScripts();

/**
 * @brief getOnTimedUpdateEventScripts Gets a map of scripts that are useful to regularly periodically call i.e. on update
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getOnTimedUpdateEventScripts();

/**
 * @brief getTaskQueueBatchProcessingScripts Gets a map of scripts used for batch processing multiple images on the task queue
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getTaskQueueBatchProcessingScripts();

}

}
