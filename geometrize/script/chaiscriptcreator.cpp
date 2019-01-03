#include "chaiscriptcreator.h"

#include "chaiscript/chaiscript.hpp"

#include "script/bindings/bindingscreator.h"

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

    chai->add(bindings::createDefaultBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createLaunchWindowEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createLaunchWindowBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createImageTaskEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createGeometrizeLibraryBindings());

    chai->add(bindings::createImageBindings());
    chai->add(bindings::createImageTaskBindings());
    chai->add(bindings::createImageExportBindings());
    chai->add(bindings::createAnimatedGifExportBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createShapeMutatorEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createGeometrizeLibraryBindings());
    chai->add(bindings::createMathBindings());

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createFunctionalTestRunnerEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    addDefaultTypes(chai);

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createLaunchWindowBindings());
    chai->add(bindings::createImageTaskBindings());
    chai->add(bindings::createUserInterfacePuppeteerBindings());

    return chai;
}

}

}
