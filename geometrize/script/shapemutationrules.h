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
     * @brief setup Sets the setup and mutation rules owned by this shape mutation rules object on the given shape mutator.
     * @param mutator The shape mutator that will use the rules.
     */
    void setup(geometrize::ShapeMutator& mutator);

    void setShapeRegion() {} // TODO
    // TODO override the shape orientation, force it to be a particular angle?
    // TODO minimum sizes etc?

private:
    class ShapeMutationRulesImpl;
    std::unique_ptr<ShapeMutationRulesImpl> d;
};

}
