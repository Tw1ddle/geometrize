#include "chaiscriptcreator.h"

#include "chaiscript/chaiscript.hpp"

#include "script/bindingscreator.h"

namespace
{

void addPrintRedirect(const std::unique_ptr<chaiscript::ChaiScript>& chai)
{
    chai->eval(R"(global print = fun(x) { printToConsole(to_string(x)); })"); // Redirect prints to console
}

void addDefaultTypes(const std::unique_ptr<chaiscript::ChaiScript>& chai)
{
    chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<int>>("IntVector"));
    chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::string>>("StringVector"));
}

}

namespace geometrize
{

namespace script
{

std::unique_ptr<chaiscript::ChaiScript> createDefaultEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(createDefaultBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createLaunchWindowEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(createDefaultBindings());
    chai->add(createLaunchWindowBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createImageTaskEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(createDefaultBindings());
    chai->add(createGeometrizeLibraryBindings());

    chai->add(createImageBindings());
    chai->add(createImageTaskBindings());
    chai->add(createImageExportBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createShapeMutatorEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(createDefaultBindings());
    chai->add(createGeometrizeLibraryBindings());
    chai->add(createMathBindings());

    return chai;
}

}

}
