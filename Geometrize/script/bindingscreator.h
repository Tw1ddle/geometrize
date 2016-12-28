#pragma once

#include <memory>

namespace chaiscript
{
class Module;
}

namespace geometrize
{

namespace script
{

/**
 * @brief createBindings Creates the Chaiscript to C++ bindings.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createBindings();

}

}
