#pragma once

#include <map>
#include <memory>
#include <string>

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{
class ShapeMutator;
}

namespace geometrize
{

namespace script
{

/**
 * @brief The GeometrizerEngine class encapsulates setup and mutation methods for geometrizing shapes.
 */
class GeometrizerEngine
{
public:
    GeometrizerEngine();
    GeometrizerEngine& operator=(const GeometrizerEngine&) = delete;
    GeometrizerEngine(const GeometrizerEngine&) = delete;
    ~GeometrizerEngine();

    /**
     * @brief getEngine Gets a pointer to the script engine used by the shape mutation engine.
     * @return A pointer to the script engine used by the shape mutation engine.
     */
    chaiscript::ChaiScript* getEngine();

    /**
     * @brief setMutator Sets the shape mutator whose shape mutation functions will be managed by the geometrizer engine.
     * @param mutator The shape mutator.
     */
    void setMutator(geometrize::ShapeMutator* mutator);

    /**
     * @brief setEnabled Enables or disables the engine. If disabled, the shape mutator will use its hardcoded defaults instead of the engine implementations.
     * @param enabled True to enable the engine, false to disable it.
     */
    void setEnabled(bool enabled);

    /**
     * @brief setupScripts Sets the scripted setup and mutation rules on the current shape mutator.
     * @param functions A map of function identifiers to Chaiscript code that will be added to the engine.
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void setupScripts(const std::map<std::string, std::string>& functions);

    /**
     * @brief resetEngine Resets the scripts and the script engine (including global variables) to their defaults.
     * @param functions A map of function identifiers to Chaiscript code that will be added to the engine (in case we have to override any of the defaults).
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void resetEngine(const std::map<std::string, std::string>& functions);

    //void setPermittedShapeRegion() {}
    //void setIntProperty(const std::string& propName, int value);

private:
    class GeometrizerEngineImpl;
    std::unique_ptr<GeometrizerEngineImpl> d;
};

}

}
