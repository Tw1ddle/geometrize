#pragma once

#include <map>
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
     * @brief setupScripts Sets the default scripted setup and mutation rules on the given shape mutator.
     * @param mutator The shape mutator that will use the rules.
     * @param functions A map of function identifiers to Chaiscript code that will be added to the engine.
     * Note that this must only be called when the
     */
    void setupScripts(geometrize::ShapeMutator& mutator, const std::map<std::string, std::string>& functions);

    //void setPermittedShapeRegion() {}
    //void setIntProperty(const std::string& propName, int value);

    // TODO override the shape orientation, force it to be a particular angle?
    // TODO minimum sizes etc?

private:
    class ShapeMutationRulesImpl;
    std::unique_ptr<ShapeMutationRulesImpl> d;
};

}
