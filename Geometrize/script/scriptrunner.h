#pragma once

#include <string>

#include "script/scriptoptions.h"

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
 * @param script The script code to evaluate.
 * @param runner The engine that will evaluate the script.
 * @param options The options used for running the script.
 */
void runScript(const std::string& script, chaiscript::ChaiScript& runner, const geometrize::script::ScriptOptions& options = geometrize::script::ScriptOptions());

/**
 * @brief runScript Evaluates the provided script code, creating a fresh engine to evaluate the script.
 * @param script The script code to evaluate.
 * @param options The options used for running the script.
 */
void runScript(const std::string& script, const ScriptOptions& options = geometrize::script::ScriptOptions());

}

}
