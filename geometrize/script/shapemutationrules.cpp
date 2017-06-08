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
        addSetupFunction<geometrize::Circle>(mutator, "setupCircle");
        addSetupFunction<geometrize::Ellipse>(mutator, "setupEllipse");
        addSetupFunction<geometrize::Line>(mutator, "setupLine");
        addSetupFunction<geometrize::Polyline>(mutator, "setupPolyline");
        addSetupFunction<geometrize::QuadraticBezier>(mutator, "setupQuadraticBezier");
        addSetupFunction<geometrize::Rectangle>(mutator, "setupRectangle");
        addSetupFunction<geometrize::RotatedEllipse>(mutator, "setupRotatedEllipse");
        addSetupFunction<geometrize::RotatedRectangle>(mutator, "setupRotatedRectangle");
        addSetupFunction<geometrize::Triangle>(mutator, "setupTriangle");

        addMutatorFunction<geometrize::Circle>(mutator, "mutateCircle");
        addMutatorFunction<geometrize::Ellipse>(mutator, "mutateEllipse");
        addMutatorFunction<geometrize::Line>(mutator, "mutateLine");
        addMutatorFunction<geometrize::Polyline>(mutator, "mutatePolyline");
        addMutatorFunction<geometrize::QuadraticBezier>(mutator, "mutateQuadraticBezier");
        addMutatorFunction<geometrize::Rectangle>(mutator, "mutateRectangle");
        addMutatorFunction<geometrize::RotatedEllipse>(mutator, "mutateRotatedEllipse");
        addMutatorFunction<geometrize::RotatedRectangle>(mutator, "mutateRotatedRectangle");
        addMutatorFunction<geometrize::Triangle>(mutator, "mutateTriangle");
    }

    // TODO need to load scripts from saved settings/config files - TODO should do this in the same manner as loading changes to text in the GUI
    void setCustomScriptedMutators(const std::vector<std::pair<std::string, std::string>>& functionNameCodePairs)
    {
        for(const std::pair<std::string, std::string>& pair : functionNameCodePairs) {

        };
    }

private:
    template <class T>
    void addSetupFunction(geometrize::ShapeMutator& mutator, const std::string& functionName)
    {
        addDefaultFunction(functionName);
        mutator.setSetupFunction(m_engine->eval<std::function<void(T&)>>(functionName));
    }

    template <class T>
    void addMutatorFunction(geometrize::ShapeMutator& mutator, const std::string& functionName)
    {
        addDefaultFunction(functionName);
        mutator.setMutatorFunction(m_engine->eval<std::function<void(T&)>>(functionName));
    }

    void addDefaultFunction(const std::string& functionName)
    {
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

void ShapeMutationRules::setupLibraryDefaults(geometrize::ShapeMutator& mutator)
{
    d->setupLibraryDefaults(mutator);
}

void ShapeMutationRules::setupScriptedDefaults(geometrize::ShapeMutator& mutator)
{
    d->setupScriptedDefaults(mutator);
}

}
