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
 * @folder Path to the folder containing the ChaiScript scripts to retrieve. The files must have the ".chai" file extension.
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
 * @brief getStopConditionScripts Gets a map of scripts that evaluate to true/false, and can be used to check whether to stop an image task (usually after stepping).
 * @return A map of Chaiscript script function names to function bodies.
 */
std::map<std::string, std::string> getStopConditionScripts();

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

}

}
