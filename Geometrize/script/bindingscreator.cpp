#include "script/bindingscreator.h"

#include "chaiscript/chaiscript.hpp"

#include "bindingswrapper.h"

#define ADD_FREE_FUN(Name) try { module->add(chaiscript::fun(&Name), #Name); } catch(...) { assert(0 && #Name); }
#define ADD_MEM_FUN(Class, Name) try { module->add(chaiscript::fun(&Class::Name), #Name) } catch(...) { assert(0 && #Name); }

namespace geometrize
{

namespace script
{

std::shared_ptr<chaiscript::Module> createBindings()
{
    auto module{std::make_shared<chaiscript::Module>()};

    // Setup bindings
    ADD_FREE_FUN(printToConsole);
    ADD_FREE_FUN(directoryContainsFile);
    ADD_FREE_FUN(directoryExists);
    ADD_FREE_FUN(readFileAsString);
    ADD_FREE_FUN(getFilePathsForDirectory);

    ADD_FREE_FUN(getScriptSearchPaths);
    ADD_FREE_FUN(getTemplateSearchPaths);

    ADD_FREE_FUN(getApplicationDirectoryPath);

    ADD_FREE_FUN(getFirstFileWithExtension);
    ADD_FREE_FUN(getScriptsForPath);

    ADD_FREE_FUN(openJob);

    return module;
}

}

}
