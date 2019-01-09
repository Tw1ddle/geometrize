#include "geometrizerengine.h"

#include <functional>
#include <map>
#include <memory>
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
#include "geometrize/shape/shape.h"
#include "geometrize/shape/shapefactory.h"
#include "geometrize/shape/triangle.h"

#include "common/util.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "script/scriptutil.h"

namespace geometrize
{

namespace script
{

class GeometrizerEngine::GeometrizerEngineImpl
{
public:
    GeometrizerEngineImpl(GeometrizerEngine* pQ) : q{pQ}, m_defaultScripts{script::getDefaultScripts()}, m_engine{script::createShapeMutatorEngine()}
    {
        m_state = m_engine->get_state();
    }
    ~GeometrizerEngineImpl() = default;
    GeometrizerEngineImpl& operator=(const GeometrizerEngineImpl&) = default;
    GeometrizerEngineImpl(const GeometrizerEngineImpl&) = default;

    std::function<std::shared_ptr<geometrize::Shape>()> makeShapeCreator()
    {
        // TODO implement types/bindings based on the actual preferences + bindings
        //return geometrize::createDefaultShapeCreator(geometrize::ShapeTypes::CIRCLE, 64, 64);
        return nullptr;
    }

    chaiscript::ChaiScript* getEngine()
    {
        return m_engine.get();
    }

    void setupScripts(const std::map<std::string, std::string>& functions)
    {
        resetState(functions);
    }

    void resetEngine(const std::map<std::string, std::string>& functions)
    {
        resetState(functions);
    }

private:
    void resetState(const std::map<std::string, std::string>& customFunctions)
    {
        m_engine->set_state(m_state); // Restore to the original engine state, this wipes out the function(s) we need to redefine.

        emit q->signal_didResetState();

        // Starting from the base state, re-add custom functions, then attempt to add missing required ones with defaults.
        // This is an ugly workaround, seems to be no choice because Chaiscript does not let us reload/redefine functions easily.
        for(const auto& entry : customFunctions) {
            try {
                m_engine->eval(entry.second);
                q->signal_scriptEvaluationSucceeded(entry.first, entry.second);
            } catch(const chaiscript::exception::eval_error& e) {
                q->signal_scriptEvaluationFailed(entry.first, entry.second, e.pretty_print());
            } catch(...) {
                q->signal_scriptEvaluationFailed(entry.first, entry.second, "Unknown script evaluation error");
            }
        }

        // Ensure all the default functions have been set (these map to the scripts from the resource files)
        installDefaults();
    }

    void installDefaults()
    {
        installDefault<geometrize::Circle>("setupCircle");
        installDefault<geometrize::Ellipse>("setupEllipse");
        installDefault<geometrize::Line>("setupLine");
        installDefault<geometrize::Polyline>("setupPolyline");
        installDefault<geometrize::QuadraticBezier>("setupQuadraticBezier");
        installDefault<geometrize::Rectangle>("setupRectangle");
        installDefault<geometrize::RotatedEllipse>("setupRotatedEllipse");
        installDefault<geometrize::RotatedRectangle>("setupRotatedRectangle");
        installDefault<geometrize::Triangle>("setupTriangle");

        installDefault<geometrize::Circle>("mutateCircle");
        installDefault<geometrize::Ellipse>("mutateEllipse");
        installDefault<geometrize::Line>("mutateLine");
        installDefault<geometrize::Polyline>("mutatePolyline");
        installDefault<geometrize::QuadraticBezier>("mutateQuadraticBezier");
        installDefault<geometrize::Rectangle>("mutateRectangle");
        installDefault<geometrize::RotatedEllipse>("mutateRotatedEllipse");
        installDefault<geometrize::RotatedRectangle>("mutateRotatedRectangle");
        installDefault<geometrize::Triangle>("mutateTriangle");
    }

    template<class T>
    void installDefault(const std::string& functionName)
    {
        try {
            const auto it{m_defaultScripts.find(functionName)};
            assert(it != m_defaultScripts.end());
            m_engine->eval(it->second);
        } catch(...) {
            // Either syntax error or the function was already defined
            //assert(0 && "Encountered script error when adding default shape function");
        }

        const auto f{m_engine->eval<std::function<void(T&)>>(functionName)};

        const std::string setupPrefix{"setup"};
        const std::string mutatePrefix{"mutate"};
        if(functionName.find(setupPrefix) == 0) {
            //m_mutator->setSetupFunction(f);
            return;
        } else if(functionName.find(mutatePrefix) == 0) {
            //m_mutator->setMutatorFunction(f);
            return;
        }

        assert(0 && "Checking for unrecognized required function, will ignore it");
    }

    GeometrizerEngine* q;
    const std::map<std::string, std::string> m_defaultScripts; ///< The default/fallbacks scripts loaded from the resources folder (function name and fields).
    std::unique_ptr<chaiscript::ChaiScript> m_engine;
    chaiscript::ChaiScript::State m_state;
};

GeometrizerEngine::GeometrizerEngine() : d{std::make_unique<GeometrizerEngine::GeometrizerEngineImpl>(this)}
{
}

GeometrizerEngine::~GeometrizerEngine()
{
}

std::function<std::shared_ptr<geometrize::Shape>()> GeometrizerEngine::makeShapeCreator()
{
    return d->makeShapeCreator();
}

chaiscript::ChaiScript* GeometrizerEngine::getEngine()
{
    return d->getEngine();
}

void GeometrizerEngine::setupScripts(const std::map<std::string, std::string>& functions)
{
    d->setupScripts(functions);
}

void GeometrizerEngine::resetEngine(const std::map<std::string, std::string>& functions)
{
    d->resetEngine(functions);
}

}

}
