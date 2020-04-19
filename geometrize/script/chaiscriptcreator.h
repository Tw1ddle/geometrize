#pragma once

#include <memory>

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace script
{

/**
 * @brief createDefaultEngine Creates a default Chaiscript engine object, with the default bindings (Geometrize-specific utility functions etc).
 * @return The Chaiscript engine object.
 */
std::unique_ptr<chaiscript::ChaiScript> createDefaultEngine();

/**
 * @brief createLaunchWindowEngine Creates a Chaiscript engine object for the launch window.
 * @return The Chaiscript engine object.
 */
std::unique_ptr<chaiscript::ChaiScript> createLaunchWindowEngine();

/**
 * @brief createImageTaskEngine Creates a Chaiscript engine for an image task window.
 * @return The Chaiscript engine object.
 */
std::unique_ptr<chaiscript::ChaiScript> createImageTaskEngine();

/**
 * @brief createBatchImageTaskEngine Creates a Chaiscript engine for creating and processing multiple image tasks.
 * @return The Chaiscript engine object.
 */
std::unique_ptr<chaiscript::ChaiScript> createBatchImageTaskEngine();

/**
 * @brief createShapeMutatorEngine Creates a Chaiscript engine object for mutating shapes.
 * @return The Chaiscript engine object.
 */
std::unique_ptr<chaiscript::ChaiScript> createShapeMutatorEngine();

/**
 * @brief createFunctionalTestRunnerEngine Creates a Chaiscript engine object for running functional tests.
 * @return The Chaiscript engine object.
 */
std::unique_ptr<chaiscript::ChaiScript> createFunctionalTestRunnerEngine();

}

}
