#include "chaiscriptcreator.h"

#include "chaiscript/chaiscript.hpp"
#include "chaiscript/chaiscript_stdlib.hpp"

#include "bindingscreator.h"

namespace geometrize
{

namespace script
{

std::unique_ptr<chaiscript::ChaiScript> createChaiScript()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<int>>("IntVector"));
    chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::string>>("StringVector"));

    chai->add(createBindings());

    chai->eval(R"(global print = fun(x) { printToConsole(to_string(x)); })"); // Redirect prints to console

    return chai;
}

}

}
