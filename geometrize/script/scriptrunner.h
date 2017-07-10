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
class ScriptOptions;
}

}

namespace geometrize
{

namespace script
{

/**
 * @brief runScript Evaluates the provided script code.
 * @param code The script code to evaluate.
 * @param runner The engine that will evaluate the script.
 * @param options The options used for running the script.
 */
void runScript(const std::string& code, chaiscript::ChaiScript& runner, const geometrize::script::ScriptOptions* const options = nullptr);

/**
 * @brief runScript Evaluates the provided script code, creating a fresh engine to evaluate the script.
 * @param code The script code to evaluate.
 * @param options The options used for running the script.
 */
void runScript(const std::string& code, const ScriptOptions* const options = nullptr);

}

}
