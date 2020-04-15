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

}

}
