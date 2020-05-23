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
#include "geometrize/core.h"
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
    installShapeScript<geometrize::Circle>("setupCircle", scripts);
    installShapeScript<geometrize::Ellipse>("setupEllipse", scripts);
    installShapeScript<geometrize::Line>("setupLine", scripts);
    installShapeScript<geometrize::Polyline>("setupPolyline", scripts);
    installShapeScript<geometrize::QuadraticBezier>("setupQuadraticBezier", scripts);
    installShapeScript<geometrize::Rectangle>("setupRectangle", scripts);
    installShapeScript<geometrize::RotatedEllipse>("setupRotatedEllipse", scripts);
    installShapeScript<geometrize::RotatedRectangle>("setupRotatedRectangle", scripts);
    installShapeScript<geometrize::Triangle>("setupTriangle", scripts);

    installShapeScript<geometrize::Circle>("mutateCircle", scripts);
    installShapeScript<geometrize::Ellipse>("mutateEllipse", scripts);
    installShapeScript<geometrize::Line>("mutateLine", scripts);
    installShapeScript<geometrize::Polyline>("mutatePolyline", scripts);
    installShapeScript<geometrize::QuadraticBezier>("mutateQuadraticBezier", scripts);
    installShapeScript<geometrize::Rectangle>("mutateRectangle", scripts);
    installShapeScript<geometrize::RotatedEllipse>("mutateRotatedEllipse", scripts);
    installShapeScript<geometrize::RotatedRectangle>("mutateRotatedRectangle", scripts);
    installShapeScript<geometrize::Triangle>("mutateTriangle", scripts);

    const auto iter = scripts.find("customEnergyFunction");
    if(iter != scripts.end()) {
        installCustomEnergyScript(iter->first, iter->second);
    }
}

}

}
