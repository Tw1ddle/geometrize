#include "chaiscriptcreator.h"

#include <utility>
#include <vector>

#include "chaiscript/chaiscript.hpp"

#include "script/bindings/bindingscreator.h"

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

    chai->add(bindings::createDefaultBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createLaunchWindowEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createQWidgetBindings());
    chai->add(bindings::createLaunchWindowBindings());
    chai->add(bindings::createImageTaskWindowBindings());
    chai->add(bindings::createSynchronousImageTaskBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createImageTaskEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createGeometrizeLibraryBindings());

    chai->add(bindings::createImageBindings());
    chai->add(bindings::createImageTaskBindings());
    chai->add(bindings::createSynchronousImageTaskBindings());
    chai->add(bindings::createImageExportBindings());
    chai->add(bindings::createAnimatedGifExportBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createBatchImageTaskEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createGeometrizeLibraryBindings());

    chai->add(bindings::createImageBindings());
    chai->add(bindings::createImageTaskBindings());
    chai->add(bindings::createSynchronousImageTaskBindings());
    chai->add(bindings::createImageTaskWindowBindings());
    chai->add(bindings::createImageExportBindings());

    addPrintRedirect(chai);

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createShapeMutatorEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createGeometrizeLibraryBindings());
    chai->add(bindings::createMathBindings());
    chai->add(bindings::createTabletEventBindings());
    chai->add(bindings::createImageExportBindings());
    chai->add(bindings::createImageTaskBindings());

    return chai;
}

std::unique_ptr<chaiscript::ChaiScript> createFunctionalTestRunnerEngine()
{
    std::unique_ptr<chaiscript::ChaiScript> chai = std::make_unique<chaiscript::ChaiScript>();

    chai->add(bindings::createDefaultBindings());
    chai->add(bindings::createQWidgetBindings());
    chai->add(bindings::createLaunchWindowBindings());
    chai->add(bindings::createImageTaskBindings());
    chai->add(bindings::createSynchronousImageTaskBindings());
    chai->add(bindings::createUserInterfacePuppeteerBindings());
    chai->add(bindings::createFunctionalTestHelperBindings());

    return chai;
}

}

}
