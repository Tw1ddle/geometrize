#pragma once

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
 * @brief getEngineFunctionNames Gets the names of the functions registered with the given engine.
 * @param engine The engine whose function names will be fetched.
 * @return The names registered with the given engine.
 */
std::vector<std::string> getEngineFunctionNames(const chaiscript::ChaiScript& engine);

}

}
