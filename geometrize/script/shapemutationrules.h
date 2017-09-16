#pragma once

#include <map>
#include <memory>

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

/**
 * @brief The ShapeMutationRules class encapsulates setup and mutation rules for shapes.
 */
class ShapeMutationRules
{
public:
    ShapeMutationRules();
    ShapeMutationRules& operator=(const ShapeMutationRules&) = delete;
    ShapeMutationRules(const ShapeMutationRules&) = delete;
    ~ShapeMutationRules();

    /**
     * @brief getEngine Gets a pointer to the engine used by the shape mutation rules.
     * @return A pointer to the engine used by the shape mutation rules.
     */
    chaiscript::ChaiScript* getEngine();

    /**
     * @brief setupScripts Sets the default scripted setup and mutation rules on the given shape mutator.
     * @param mutator The shape mutator that will use the rules.
     * @param functions A map of function identifiers to Chaiscript code that will be added to the engine.
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void setupScripts(geometrize::ShapeMutator& mutator, const std::map<std::string, std::string>& functions);

    /**
     * @brief resetScriptsToDefaults Reset the scripts to their default implementations.
     * @param mutator The shape mutator that will use the rules.
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void resetScriptsToDefaults(geometrize::ShapeMutator& mutator);

    /**
     * @brief resetEngineToDefaults Resets the scripts and their entire engine to their defaults.
     * @param mutator The shape mutator that will use the rules.
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void resetEngineToDefaults(geometrize::ShapeMutator& mutator);

    //void setPermittedShapeRegion() {}
    //void setIntProperty(const std::string& propName, int value);

    // TODO override the shape orientation, force it to be a particular angle?
    // TODO minimum sizes etc?

private:
    class ShapeMutationRulesImpl;
    std::unique_ptr<ShapeMutationRulesImpl> d;
};

}
