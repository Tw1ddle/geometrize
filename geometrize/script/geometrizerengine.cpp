#include "geometrizerengine.h"

#include <cstdint>
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
#include "geometrize/rasterizer/rasterizer.h"
#include "geometrize/shape/rectangle.h"
#include "geometrize/shape/rotatedellipse.h"
#include "geometrize/shape/rotatedrectangle.h"
#include "geometrize/shape/shape.h"
#include "geometrize/shape/shapefactory.h"
#include "geometrize/shape/shapetypes.h"
#include "geometrize/shape/triangle.h"

#include "common/util.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "script/scriptutil.h"

namespace geometrize
{

namespace script
{

class ShapeCreator
{
public:
    ShapeCreator()
    {
    }

    std::function<std::shared_ptr<geometrize::Shape>()> createShapeCreator(const geometrize::ShapeTypes types, const std::int32_t w, const std::int32_t h)
    {
        auto f = [this, types, w, h]() {
            std::shared_ptr<geometrize::Shape> s = geometrize::randomShapeOf(types);

            switch(s->getType()) {
            case geometrize::ShapeTypes::RECTANGLE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::Rectangle&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::Rectangle&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Rectangle&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::ROTATED_RECTANGLE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::RotatedRectangle&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::RotatedRectangle&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::RotatedRectangle&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::TRIANGLE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::Triangle&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::Triangle&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Triangle&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::ELLIPSE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::Ellipse&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::Ellipse&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Ellipse&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::ROTATED_ELLIPSE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::RotatedEllipse&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::RotatedEllipse&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::RotatedEllipse&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::CIRCLE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::Circle&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::Circle&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Circle&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::LINE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::Line&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::Line&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Line&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::QUADRATIC_BEZIER: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::QuadraticBezier&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::QuadraticBezier&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::QuadraticBezier&>(s), w, h); };
                break;
            }
            case geometrize::ShapeTypes::POLYLINE: {
                s->setup = [this](geometrize::Shape& s) { return setup(static_cast<geometrize::Polyline&>(s)); };
                s->mutate = [this](geometrize::Shape& s) { mutate(static_cast<geometrize::Polyline&>(s)); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Polyline&>(s), w, h); };
                break;
            }
            default:
                assert(0 && "Bad shape type");
            }

            return s;
        };

        return f;
    }

    void setup(geometrize::Circle& shape) const
    {
        m_setupCircle(shape);
    }

    void mutate(geometrize::Circle& shape) const
    {
        m_mutateCircle(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::Circle&)>& f)
    {
        m_setupCircle = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::Circle&)>& f)
    {
        m_mutateCircle = f;
    }

    void setup(geometrize::Ellipse& shape) const
    {
        m_setupEllipse(shape);
    }

    void mutate(geometrize::Ellipse& shape) const
    {
        m_mutateEllipse(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::Ellipse&)>& f)
    {
        m_setupEllipse = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::Ellipse&)>& f)
    {
        m_mutateEllipse = f;
    }

    void setup(geometrize::Line& shape) const
    {
        m_setupLine(shape);
    }

    void mutate(geometrize::Line& shape) const
    {
        m_mutateLine(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::Line&)>& f)
    {
        m_setupLine = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::Line&)>& f)
    {
        m_mutateLine = f;
    }

    void setup(geometrize::Polyline& shape) const
    {
        m_setupPolyline(shape);
    }

    void mutate(geometrize::Polyline& shape) const
    {
        m_mutatePolyline(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::Polyline&)>& f)
    {
        m_setupPolyline = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::Polyline&)>& f)
    {
        m_mutatePolyline = f;
    }

    void setup(geometrize::QuadraticBezier& shape) const
    {
        m_setupQuadraticBezier(shape);
    }

    void mutate(geometrize::QuadraticBezier& shape) const
    {
        m_mutateQuadraticBezier(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::QuadraticBezier&)>& f)
    {
        m_setupQuadraticBezier = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::QuadraticBezier&)>& f)
    {
        m_mutateQuadraticBezier = f;
    }

    void setup(geometrize::Rectangle& shape) const
    {
        m_setupRectangle(shape);
    }

    void mutate(geometrize::Rectangle& shape) const
    {
        m_mutateRectangle(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::Rectangle&)>& f)
    {
        m_setupRectangle = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::Rectangle&)>& f)
    {
        m_mutateRectangle = f;
    }

    void setup(geometrize::RotatedEllipse& shape) const
    {
        m_setupRotatedEllipse(shape);
    }

    void mutate(geometrize::RotatedEllipse& shape) const
    {
        m_mutateRotatedEllipse(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::RotatedEllipse&)>& f)
    {
        m_setupRotatedEllipse = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::RotatedEllipse&)>& f)
    {
        m_mutateRotatedEllipse = f;
    }

    void setup(geometrize::RotatedRectangle& shape) const
    {
        m_setupRotatedRectangle(shape);
    }

    void mutate(geometrize::RotatedRectangle& shape) const
    {
        m_mutateRotatedRectangle(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::RotatedRectangle&)>& f)
    {
        m_setupRotatedRectangle = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::RotatedRectangle&)>& f)
    {
        m_mutateRotatedRectangle = f;
    }

    void setup(geometrize::Triangle& shape) const
    {
        m_setupTriangle(shape);
    }

    void mutate(geometrize::Triangle& shape) const
    {
        m_mutateTriangle(shape);
    }

    void setSetupFunction(const std::function<void(geometrize::Triangle&)>& f)
    {
        m_setupTriangle = f;
    }

    void setMutatorFunction(const std::function<void(geometrize::Triangle&)>& f)
    {
        m_mutateTriangle = f;
    }

private:
    std::function<void(geometrize::Circle&)> m_setupCircle;
    std::function<void(geometrize::Ellipse&)> m_setupEllipse;
    std::function<void(geometrize::Line&)> m_setupLine;
    std::function<void(geometrize::Polyline&)> m_setupPolyline;
    std::function<void(geometrize::QuadraticBezier&)> m_setupQuadraticBezier;
    std::function<void(geometrize::Rectangle&)> m_setupRectangle;
    std::function<void(geometrize::RotatedEllipse&)> m_setupRotatedEllipse;
    std::function<void(geometrize::RotatedRectangle&)> m_setupRotatedRectangle;
    std::function<void(geometrize::Triangle&)> m_setupTriangle;

    std::function<void(geometrize::Circle&)> m_mutateCircle;
    std::function<void(geometrize::Ellipse&)> m_mutateEllipse;
    std::function<void(geometrize::Line&)> m_mutateLine;
    std::function<void(geometrize::Polyline&)> m_mutatePolyline;
    std::function<void(geometrize::QuadraticBezier&)> m_mutateQuadraticBezier;
    std::function<void(geometrize::Rectangle&)> m_mutateRectangle;
    std::function<void(geometrize::RotatedEllipse&)> m_mutateRotatedEllipse;
    std::function<void(geometrize::RotatedRectangle&)> m_mutateRotatedRectangle;
    std::function<void(geometrize::Triangle&)> m_mutateTriangle;
};

class GeometrizerEngine::GeometrizerEngineImpl
{
public:
    GeometrizerEngineImpl(GeometrizerEngine* pQ) : q{pQ}, m_defaultScripts{script::getDefaultShapeMutatorScripts()}, m_engine{script::createShapeMutatorEngine()}
    {
        m_state = m_engine->get_state();

        resetState({});
    }
    ~GeometrizerEngineImpl() = default;
    GeometrizerEngineImpl& operator=(const GeometrizerEngineImpl&) = default;
    GeometrizerEngineImpl(const GeometrizerEngineImpl&) = default;

    std::function<std::shared_ptr<geometrize::Shape>()> makeShapeCreator(const geometrize::ShapeTypes types, const std::int32_t w, const std::int32_t h)
    {
        return m_creator.createShapeCreator(types, w, h);
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
                m_engine->parse(entry.second);
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

        const auto g = [f](T& s) {
            try {
                f(s);
            } catch(std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        };

        const std::string setupPrefix{"setup"};
        const std::string mutatePrefix{"mutate"};
        if(functionName.find(setupPrefix) == 0) {
            m_creator.setSetupFunction(g);
            return;
        } else if(functionName.find(mutatePrefix) == 0) {
            m_creator.setMutatorFunction(g);
            return;
        }

        assert(0 && "Checking for unrecognized required function, will ignore it");
    }

    GeometrizerEngine* q;
    const std::map<std::string, std::string> m_defaultScripts; ///< The default/fallbacks scripts loaded from the resources folder (function name and fields).
    std::unique_ptr<chaiscript::ChaiScript> m_engine;
    chaiscript::ChaiScript::State m_state;
    ShapeCreator m_creator;
};

GeometrizerEngine::GeometrizerEngine() : d{std::make_unique<GeometrizerEngine::GeometrizerEngineImpl>(this)}
{
}

GeometrizerEngine::~GeometrizerEngine()
{
}

std::function<std::shared_ptr<geometrize::Shape>()> GeometrizerEngine::makeShapeCreator(const geometrize::ShapeTypes types, const std::int32_t w, const std::int32_t h)
{
    return d->makeShapeCreator(types, w, h);
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
