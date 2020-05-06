#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include <QObject>

#include "chaiscript/chaiscript.hpp"

#include "geometrize/shape/shapetypes.h"
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

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{
class Shape;
}

namespace geometrize
{

namespace script
{

/**
 * @brief The GeometrizerEngine class encapsulates script-based setup and mutation methods for geometrizing shapes.
 */
class GeometrizerEngine : public QObject, public std::enable_shared_from_this<GeometrizerEngine>
{
    Q_OBJECT

public:
    GeometrizerEngine();
    GeometrizerEngine(const chaiscript::ChaiScript::State& state);
    virtual ~GeometrizerEngine() = default;
    GeometrizerEngine& operator=(const GeometrizerEngine&) = delete;
    GeometrizerEngine(const GeometrizerEngine&) = delete;

    void installScripts(const std::map<std::string, std::string>& scripts);

    /**
     * @brief makeShapeCreator Returns a function that generates shapes for the core geometrization algorithm
     * This is passed to the geometrization algorithm and called across many threads. So it's unsafe to change any
     * state used by the script engine while we're busy adding shapes. It also captures "this", meaning the engine won't
     * be destroyed until the shape creation code is finished using it.
     * @return A function that generates shapes for the core geometrization algorithm, for passing to an ImageRunner.
     */
    std::function<std::shared_ptr<geometrize::Shape>()> makeShapeCreator(geometrize::ShapeTypes types, std::int32_t w, std::int32_t h) const
    {
        auto self(shared_from_this());
        auto f = [self, this, types, w, h]() {
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

    /**
     * @brief getEngine Gets a pointer to the script engine used by the shape mutation engine.
     * @return A pointer to the script engine used by the shape mutation engine.
     */
    chaiscript::ChaiScript* getEngine();

signals:
    /**
     * @brief signal_scriptEvaluationSucceeded Signal dispatched when a script is successfully parsed/evaluated.
     * @param functionName The name of the function.
     * @param code The code of the function.
     */
    void signal_scriptEvaluationSucceeded(const std::string& functionName, const std::string& code);

    /**
     * @brief signal_scriptEvaluationFailed Signal dispatched when a script fails to parse/evaluate.
     * @param functionName The name of the function.
     * @param code The code of the function.
     * @param error The text of the error message, if any.
     */
    void signal_scriptEvaluationFailed(const std::string& functionName, const std::string& code, const std::string& errorMessage);

private:
    template<class T>
    void installScript(const std::string& functionName, const std::map<std::string, std::string>& scripts)
    {
        try {
            const auto it{scripts.find(functionName)};
            assert(it != scripts.end());
            m_engine->eval(it->second);
        } catch(...) {
            // Either syntax error or the function was already defined
            //assert(0 && "Encountered script error when adding default shape function");
        }

        try {
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
                 setSetupFunction(g);
                 return;
             } else if(functionName.find(mutatePrefix) == 0) {
                 setMutatorFunction(g);
                 return;
             }
        } catch(std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        assert(0 && "Checking for unrecognized required function, will ignore it");
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

    std::unique_ptr<chaiscript::ChaiScript> m_engine;
};

}

}
