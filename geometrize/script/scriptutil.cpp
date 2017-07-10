#include "scriptutil.h"

#include "chaiscript/chaiscript.hpp"
#include "chaiscript/language/chaiscript_engine.hpp"

namespace geometrize
{

namespace script
{

std::vector<std::string> getEngineFunctionNames(const chaiscript::ChaiScript& engine)
{
    std::vector<std::string> functions;
    const auto funcs{engine.get_state().engine_state.m_functions};
    for(auto it = funcs.begin(); it != funcs.end(); ++it) {
        functions.push_back(it->first);
    }
    return functions;
}

}

}
