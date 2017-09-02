#include "shapemutationrules.h"

#include <map>
#include <string>
#include <vector>
#include <utility>

#include <QDirIterator>

#include "chaiscript/chaiscript.hpp"

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

#include "common/util.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"

namespace geometrize
{

class ShapeMutationRules::ShapeMutationRulesImpl
{
public:
    ShapeMutationRulesImpl() : m_scriptResourceFolder{":/scripts/scripts/default_shape_mutators/"}, m_defaultScripts{readDefaultScripts()}, m_engine{createEngine()}, m_state{m_engine->get_state()} {}
    ~ShapeMutationRulesImpl() = default;
    ShapeMutationRulesImpl& operator=(const ShapeMutationRulesImpl&) = default;
    ShapeMutationRulesImpl(const ShapeMutationRulesImpl&) = default;

    chaiscript::ChaiScript* getEngine()
    {
        return m_engine.get();
    }

    void setupScripts(geometrize::ShapeMutator& mutator, const std::map<std::string, std::string>& functions)
    {
        reseatFunctions(mutator, functions);
    }

    void resetScriptsToDefaults(geometrize::ShapeMutator& mutator)
    {
        const std::map<std::string, std::string> m{};
        reseatFunctions(mutator, m);
    }

    void resetEngineToDefaults(geometrize::ShapeMutator& mutator)
    {
        m_engine = createEngine();
        setupGlobals();
    }

private:
    std::unique_ptr<chaiscript::ChaiScript> createEngine() const
    {
        std::unique_ptr<chaiscript::ChaiScript> engine{script::createShapeMutatorEngine()};
        return engine;
    }

    void setupGlobals()
    {
        // TODO
        m_engine->set_global(chaiscript::var("todo"), "foo"); // set the globals that will be used
    }

    void reseatFunctions(geometrize::ShapeMutator& mutator, const std::map<std::string, std::string>& customFunctions)
    {
        m_engine->set_state(m_state); // Restore to the original engine state, this wipes out the function(s) we need to redefine.

        // Starting from the base state, re-add custom functions, then attempt to add missing required ones with defaults.
        // This is an ugly workaround, seems to be no choice because Chaiscript does not let us reload/redefine functions easily.
        for(const auto& entry : customFunctions) {
            m_engine->eval(entry.second);
        }

        // Ensure all the default functions have been set (these map to the scripts from the resource files)
        installDefaults(mutator);
    }

    void installDefaults(geometrize::ShapeMutator& mutator)
    {
        tryAddAndBind<geometrize::Circle>(mutator, "setupCircle");
        tryAddAndBind<geometrize::Ellipse>(mutator, "setupEllipse");
        tryAddAndBind<geometrize::Line>(mutator, "setupLine");
        tryAddAndBind<geometrize::Polyline>(mutator, "setupPolyline");
        tryAddAndBind<geometrize::QuadraticBezier>(mutator, "setupQuadraticBezier");
        tryAddAndBind<geometrize::Rectangle>(mutator, "setupRectangle");
        tryAddAndBind<geometrize::RotatedEllipse>(mutator, "setupRotatedEllipse");
        tryAddAndBind<geometrize::RotatedRectangle>(mutator, "setupRotatedRectangle");
        tryAddAndBind<geometrize::Triangle>(mutator, "setupTriangle");

        tryAddAndBind<geometrize::Circle>(mutator, "mutateCircle");
        tryAddAndBind<geometrize::Ellipse>(mutator, "mutateEllipse");
        tryAddAndBind<geometrize::Line>(mutator, "mutateLine");
        tryAddAndBind<geometrize::Polyline>(mutator, "mutatePolyline");
        tryAddAndBind<geometrize::QuadraticBezier>(mutator, "mutateQuadraticBezier");
        tryAddAndBind<geometrize::Rectangle>(mutator, "mutateRectangle");
        tryAddAndBind<geometrize::RotatedEllipse>(mutator, "mutateRotatedEllipse");
        tryAddAndBind<geometrize::RotatedRectangle>(mutator, "mutateRotatedRectangle");
        tryAddAndBind<geometrize::Triangle>(mutator, "mutateTriangle");
    }

    template<class T>
    void tryAddAndBind(geometrize::ShapeMutator& mutator, const std::string& functionName)
    {
        try {
            // TODO add assertion if not found
            const std::string code{m_defaultScripts.find(functionName)->second};
            m_engine->eval(code);
        } catch(...) {
            // Either syntax error or the function was already defined
        }

        const auto f{m_engine->eval<std::function<void(T&)>>(functionName)};

        const std::string setupPrefix{"setup"};
        const std::string mutatePrefix{"mutate"};
        if(functionName.find(setupPrefix) == 0) {
            mutator.setSetupFunction(f);
        } else if(functionName.find(mutatePrefix) == 0) {
            mutator.setMutatorFunction(f);
        } else {
            assert(0 && "Checking for unrecognized required function, will ignore it");
        }
    }

    std::map<std::string, std::string> readDefaultScripts()
    {
        std::map<std::string, std::string> m;

        QDirIterator it(m_scriptResourceFolder);
        while(it.hasNext()) {
            it.next();
            const std::string fileName{it.fileName().toStdString()};
            const std::string functionName{fileName.substr(0, fileName.size() - 5)}; // Remove ".chai"
            m[functionName] = util::readFileAsString(it.filePath().toStdString());
        }

        return m;
    }

    const QString m_scriptResourceFolder; ///< Path to the default shape scripts folder in resources.
    const std::map<std::string, std::string> m_defaultScripts; ///< The default functions loaded from the resources folder (function name and fields).

    std::map<std::string, std::string> m_currentScripts; ///< The currently loaded functions loaded from the resources folder (function name and fields).

    std::unique_ptr<chaiscript::ChaiScript> m_engine;
    chaiscript::ChaiScript::State m_state;
};

ShapeMutationRules::ShapeMutationRules() : d{std::make_unique<ShapeMutationRules::ShapeMutationRulesImpl>()}
{
}

ShapeMutationRules::~ShapeMutationRules()
{
}

chaiscript::ChaiScript* ShapeMutationRules::getEngine()
{
    return d->getEngine();
}

void ShapeMutationRules::setupScripts(geometrize::ShapeMutator& mutator, const std::map<std::string, std::string>& functions)
{
    d->setupScripts(mutator, functions);
}

void ShapeMutationRules::resetScriptsToDefaults(geometrize::ShapeMutator& mutator)
{
    d->resetScriptsToDefaults(mutator);
}

void ShapeMutationRules::resetEngineToDefaults(geometrize::ShapeMutator& mutator)
{
    d->resetEngineToDefaults(mutator);
}

}
