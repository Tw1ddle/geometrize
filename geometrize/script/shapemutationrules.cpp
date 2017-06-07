#include "shapemutationrules.h"

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

    void setup(geometrize::ShapeMutator& mutator)
    {
        // TODO use a template to set the function in the addFunction method

        addFunction(mutator, "setupCircle");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::Circle&)>>("setupCircle"));
        addFunction(mutator, "setupEllipse");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::Ellipse&)>>("setupEllipse"));
        addFunction(mutator, "setupLine");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::Line&)>>("setupLine"));
        addFunction(mutator, "setupPolyline");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::Polyline&)>>("setupPolyline"));
        addFunction(mutator, "setupQuadraticBezier");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::QuadraticBezier&)>>("setupQuadraticBezier"));
        addFunction(mutator, "setupRectangle");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::Rectangle&)>>("setupRectangle"));
        addFunction(mutator, "setupRotatedEllipse");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::RotatedEllipse&)>>("setupRotatedEllipse"));
        addFunction(mutator, "setupRotatedRectangle");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::RotatedRectangle&)>>("setupRotatedRectangle"));
        addFunction(mutator, "setupTriangle");
        mutator.setSetupFunction(m_engine->eval<std::function<void(geometrize::Triangle&)>>("setupTriangle"));

        addFunction(mutator, "mutateCircle");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::Circle&)>>("mutateCircle"));
        addFunction(mutator, "mutateEllipse");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::Ellipse&)>>("mutateEllipse"));
        addFunction(mutator, "mutateLine");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::Line&)>>("mutateLine"));
        addFunction(mutator, "mutatePolyline");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::Polyline&)>>("mutatePolyline"));
        addFunction(mutator, "mutateQuadraticBezier");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::QuadraticBezier&)>>("mutateQuadraticBezier"));
        addFunction(mutator, "mutateRectangle");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::Rectangle&)>>("mutateRectangle"));
        addFunction(mutator, "mutateRotatedEllipse");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::RotatedEllipse&)>>("mutateRotatedEllipse"));
        addFunction(mutator, "mutateRotatedRectangle");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::RotatedRectangle&)>>("mutateRotatedRectangle"));
        addFunction(mutator, "mutateTriangle");
        mutator.setMutatorFunction(m_engine->eval<std::function<void(geometrize::Triangle&)>>("mutateTriangle"));
    }

private:
    void addFunction(geometrize::ShapeMutator& mutator, const std::string& functionName) {
        const std::string code{util::readFileAsString(scriptResourceFolder + functionName + ".chai")};
        m_functionCodeMap[functionName] = code;
        m_engine->eval(code); // TODO cannot redefine functions this way with chaiscript, need to recreate the whole engine, or use a global??
    }

    std::unique_ptr<chaiscript::ChaiScript> m_engine;
    std::map<std::string, std::string> m_functionCodeMap; ///< Maps function names to source code
    const std::string scriptResourceFolder{":/scripts/"}; ///< Path to the scripts folder in the resources folder
};

ShapeMutationRules::ShapeMutationRules() : d{std::make_unique<ShapeMutationRules::ShapeMutationRulesImpl>()}
{
}

ShapeMutationRules::~ShapeMutationRules()
{
}

void ShapeMutationRules::setup(geometrize::ShapeMutator& mutator)
{
    d->setup(mutator);
}

}
