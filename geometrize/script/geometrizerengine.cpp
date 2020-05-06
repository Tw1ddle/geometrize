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

GeometrizerEngine::GeometrizerEngine() : m_engine{script::createShapeMutatorEngine()}
{
}

GeometrizerEngine::GeometrizerEngine(const chaiscript::ChaiScript::State& state) : m_engine{script::createShapeMutatorEngine()}
{
    m_engine->set_state(state);
}

chaiscript::ChaiScript* GeometrizerEngine::getEngine()
{
    return m_engine.get();
}

void GeometrizerEngine::installScripts(const std::map<std::string, std::string>& scripts)
{
    installScript<geometrize::Circle>("setupCircle", scripts);
    installScript<geometrize::Ellipse>("setupEllipse", scripts);
    installScript<geometrize::Line>("setupLine", scripts);
    installScript<geometrize::Polyline>("setupPolyline", scripts);
    installScript<geometrize::QuadraticBezier>("setupQuadraticBezier", scripts);
    installScript<geometrize::Rectangle>("setupRectangle", scripts);
    installScript<geometrize::RotatedEllipse>("setupRotatedEllipse", scripts);
    installScript<geometrize::RotatedRectangle>("setupRotatedRectangle", scripts);
    installScript<geometrize::Triangle>("setupTriangle", scripts);

    installScript<geometrize::Circle>("mutateCircle", scripts);
    installScript<geometrize::Ellipse>("mutateEllipse", scripts);
    installScript<geometrize::Line>("mutateLine", scripts);
    installScript<geometrize::Polyline>("mutatePolyline", scripts);
    installScript<geometrize::QuadraticBezier>("mutateQuadraticBezier", scripts);
    installScript<geometrize::Rectangle>("mutateRectangle", scripts);
    installScript<geometrize::RotatedEllipse>("mutateRotatedEllipse", scripts);
    installScript<geometrize::RotatedRectangle>("mutateRotatedRectangle", scripts);
    installScript<geometrize::Triangle>("mutateTriangle", scripts);
}

}

}
