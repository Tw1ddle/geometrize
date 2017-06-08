#pragma once

#include <memory>

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
     * @brief setupLibraryDefaults Sets the setup and mutation rules hardcoded in the Geometrize library on the given shape mutator.
     * Far faster than other (scripted) mutation rules, but inflexible.
     * @param mutator The shape mutator that will use the rules.
     */
    void setupLibraryDefaults(geometrize::ShapeMutator& mutator);

    /**
     * @brief setupScriptedDefaults Sets the default scripted setup and mutation rules on the given shape mutator.
     * @param mutator The shape mutator that will use the rules.
     */
    void setupScriptedDefaults(geometrize::ShapeMutator& mutator);

    // TODO?
    //void setPermittedShapeRegion() {} // TODO
    //void setIntProperty(const std::string& propName, int value);

    // TODO override the shape orientation, force it to be a particular angle?
    // TODO minimum sizes etc?

private:
    class ShapeMutationRulesImpl;
    std::unique_ptr<ShapeMutationRulesImpl> d;
};

}
