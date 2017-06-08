#include "shapemutationrules.h"

#include <string>
#include <vector>
#include <utility>

#include "geometrize/commonutil.h"
#include "geometrize/model.h"
#include "geometrize/shape/circle.h"
#include "geometrize/shape/ellipse.h"
#include "geometrize/shape/line.h"
#include "geometrize/shape/polyline.h"
#include "geometrize/shape/quadraticbezier.h"
#include "geometrize/shape/rectangle.h"
#include "geometrize/shape/rotatedellipse.h"
#include "geometrize/shape/rotatedrectangle.h"
#include "geometrize/shape/triangle.h"
#include "geometrize/shape/shapemutator.h"

#include "chaiscript/chaiscript.hpp"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "util.h"

namespace geometrize
{

class ShapeMutationRules::ShapeMutationRulesImpl
{
public:
    ShapeMutationRulesImpl() : m_engine{script::createChaiScriptShapeMutator()}
    {
    }

    ~ShapeMutationRulesImpl() = default;
    ShapeMutationRulesImpl& operator=(const ShapeMutationRulesImpl&) = default;
    ShapeMutationRulesImpl(const ShapeMutationRulesImpl&) = default;

    void setupLibraryDefaults(geometrize::ShapeMutator& mutator)
    {
        mutator.setDefaults();
    }

    void setupScriptedDefaults(geometrize::ShapeMutator& mutator)
    {
        addDefaultSetupFunction<geometrize::Circle>(mutator, "setupCircle");
        addDefaultSetupFunction<geometrize::Ellipse>(mutator, "setupEllipse");
        addDefaultSetupFunction<geometrize::Line>(mutator, "setupLine");
        addDefaultSetupFunction<geometrize::Polyline>(mutator, "setupPolyline");
        addDefaultSetupFunction<geometrize::QuadraticBezier>(mutator, "setupQuadraticBezier");
        addDefaultSetupFunction<geometrize::Rectangle>(mutator, "setupRectangle");
        addDefaultSetupFunction<geometrize::RotatedEllipse>(mutator, "setupRotatedEllipse");
        addDefaultSetupFunction<geometrize::RotatedRectangle>(mutator, "setupRotatedRectangle");
        addDefaultSetupFunction<geometrize::Triangle>(mutator, "setupTriangle");

        addDefaultMutatorFunction<geometrize::Circle>(mutator, "mutateCircle");
        addDefaultMutatorFunction<geometrize::Ellipse>(mutator, "mutateEllipse");
        addDefaultMutatorFunction<geometrize::Line>(mutator, "mutateLine");
        addDefaultMutatorFunction<geometrize::Polyline>(mutator, "mutatePolyline");
        addDefaultMutatorFunction<geometrize::QuadraticBezier>(mutator, "mutateQuadraticBezier");
        addDefaultMutatorFunction<geometrize::Rectangle>(mutator, "mutateRectangle");
        addDefaultMutatorFunction<geometrize::RotatedEllipse>(mutator, "mutateRotatedEllipse");
        addDefaultMutatorFunction<geometrize::RotatedRectangle>(mutator, "mutateRotatedRectangle");
        addDefaultMutatorFunction<geometrize::Triangle>(mutator, "mutateTriangle");
    }

private:
    template <class T>
    void addDefaultSetupFunction(geometrize::ShapeMutator& mutator, const std::string& functionName)
    {
        addDefaultFunction(functionName);
        mutator.setSetupFunction(m_engine->eval<std::function<void(T&)>>(functionName));
    }

    template <class T>
    void addDefaultMutatorFunction(geometrize::ShapeMutator& mutator, const std::string& functionName)
    {
        addDefaultFunction(functionName);
        mutator.setMutatorFunction(m_engine->eval<std::function<void(T&)>>(functionName));
    }

    void addDefaultFunction(const std::string& functionName)
    {
        const std::string code{util::readFileAsString(scriptResourceFolder + functionName + ".chai")};
        m_engine->eval(code);
    }

    std::unique_ptr<chaiscript::ChaiScript> m_engine;
    const std::string scriptResourceFolder{":/scripts/"}; ///< Path to the scripts folder in the resources folder
};

ShapeMutationRules::ShapeMutationRules() : d{std::make_unique<ShapeMutationRules::ShapeMutationRulesImpl>()}
{
}

ShapeMutationRules::~ShapeMutationRules()
{
}

void ShapeMutationRules::setupLibraryDefaults(geometrize::ShapeMutator& mutator)
{
    d->setupLibraryDefaults(mutator);
}

void ShapeMutationRules::setupScriptedDefaults(geometrize::ShapeMutator& mutator)
{
    d->setupScriptedDefaults(mutator);
}

}
