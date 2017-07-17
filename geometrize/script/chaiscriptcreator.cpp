#include "chaiscriptcreator.h"

#include "chaiscript/chaiscript.hpp"

#include "bindingscreator.h"

namespace
{

void addPrintRedirect(const std::unique_ptr<chaiscript::ChaiScript>& chai)
{
    chai->eval(R"(global print = fun(x) { printToConsole(to_string(x)); })"); // Redirect prints to console
}

}

namespace geometrize
{

namespace script
{

std::unique_ptr<chaiscript::ChaiScript> createDefaultEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<int>>("IntVector"));
    chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::string>>("StringVector"));

    chai->add(createDefaultBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createLaunchWindowEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(createDefaultBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createImageJobEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(createDefaultBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createShapeMutatorEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(createDefaultBindings());
    chai->add(createShapeMutationBindings());
    chai->add(createMathBindings());

    return chai;
}

}

}
