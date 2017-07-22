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

}

}
