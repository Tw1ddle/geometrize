#pragma once

#include <string>

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace script
{

/**
 * @brief runScript Evaluates the provided script code.
 * @param code The script code to evaluate.
 * @param runner The engine that will evaluate the script.
 */
void runScript(const std::string& code, chaiscript::ChaiScript& runner);

/**
 * @brief runScript Evaluates the provided script code, creating a fresh engine to evaluate the script.
 * @param code The script code to evaluate.
 */
void runScript(const std::string& code);

}

}
