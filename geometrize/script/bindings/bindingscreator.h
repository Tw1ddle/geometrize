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

namespace bindings
{

/**
 * @brief createDefaultBindings Creates the default Chaiscript to C++ bindings.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createDefaultBindings();

/**
 * @brief createLaunchWindowBindings Creates the Chaiscript to C++ bindings for the Geometrize launch window.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createLaunchWindowBindings();

/**
 * @brief createImageTaskWindowBindings Creates the Chaiscript to C++ binding for the Geometrize image task window.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createImageTaskWindowBindings();

/**
 * @brief createImageBindings Creates the Chaiscript to C++ bindings for image manipulation.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createImageBindings();

/**
 * @brief createImageTaskBindings Creates the Chaiscript to C++ bindings for Geometrize image tasks.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createImageTaskBindings();

/**
 * @brief createImageExportBindings Creates the Chaiscript to C++ bindings for saving images.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createImageExportBindings();

/**
 * @brief createGifExportBindings Creates the ChaiScript to C++ bindings for saving animated GIFs.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createAnimatedGifExportBindings();

/**
 * @brief createGeometrizeLibraryBindings Creates the Chaiscript to C++ bindings for all of the functionality in the Geometrize library.
 * @return A shared pointer to a module enscapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createGeometrizeLibraryBindings();

/**
 * @brief createTabletEventBindings Creates the Chaiscript to C++ bindings for tablet events (e.g. Wacom pen input data).
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createTabletEventBindings();

/**
 * @brief createMathBindings Creates the Chaiscript to C++ bindings for common math functions.
 * @return A shared pointer to a module encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createMathBindings();

/**
 * @brief createUserInterfacePuppeteerBindings Creates ChaiScript to C++ bindings for driving the user interface (primarily for testing purposes).
 * @return A shared pointer encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createUserInterfacePuppeteerBindings();

/**
 * @brief createFunctionalTestHelperBindings Creates ChaiScript to C++ bindings for helper functions needed for the functional/self-tests (e.g. reporting test results).
 * @return A shared pointer encapsulating the bindings.
 */
std::shared_ptr<chaiscript::Module> createFunctionalTestHelperBindings();

}

}

}
