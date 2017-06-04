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

namespace geometrize
{

class ShapeMutationRules::ShapeMutationRulesImpl
{
public:
    ShapeMutationRulesImpl() : m_engine{script::createChaiScript()}
    {
    }

    ~ShapeMutationRulesImpl() = default;
    ShapeMutationRulesImpl& operator=(const ShapeMutationRulesImpl&) = default;
    ShapeMutationRulesImpl(const ShapeMutationRulesImpl&) = default;

    void setup(geometrize::ShapeMutator& mutator)
    {
        mutator.setSetupFunction([this](geometrize::Circle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
            shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
            shape.m_r = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1);
        });

        mutator.setSetupFunction([this](geometrize::Ellipse& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
            shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
            shape.m_rx = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1);
            shape.m_ry = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1);
        });

        mutator.setSetupFunction([this](geometrize::Line& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};

            shape.m_x1 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
            shape.m_x2 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
        });

        mutator.setSetupFunction([this](geometrize::Polyline& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};
            for(std::int32_t i = 0; i < 4; i++) {
                const std::pair<std::int32_t, std::int32_t> point{
                    geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1),
                    geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1)
                };
                shape.m_points.push_back(point);
            }
        });

        mutator.setSetupFunction([this](geometrize::QuadraticBezier& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x1 = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, xBound - 1));
            shape.m_y1 = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, yBound - 1));
            shape.m_cx = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, xBound - 1));
            shape.m_cy = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, yBound - 1));
            shape.m_x2 = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, xBound - 1));
            shape.m_y2 = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, yBound - 1));
        });

        mutator.setSetupFunction([this](geometrize::Rectangle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1), 0, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1), 0, yBound - 1);
        });

        mutator.setSetupFunction([this](geometrize::RotatedEllipse& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
            shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
            shape.m_rx = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1);
            shape.m_ry = geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1);
            shape.m_angle = geometrize::commonutil::randomRange(0, 360);
        });

        mutator.setSetupFunction([this](geometrize::RotatedRectangle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1), 0, xBound);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(0, geometrize::commonutil::randomRange(0, 32) + 1), 0, yBound);
            shape.m_angle = geometrize::commonutil::randomRange(0, 360);
        });

        mutator.setSetupFunction([this](geometrize::Triangle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
            shape.m_x2 = shape.m_x1 + geometrize::commonutil::randomRange(-32, 32);
            shape.m_y2 = shape.m_y1 + geometrize::commonutil::randomRange(-32, 32);
            shape.m_x3 = shape.m_x1 + geometrize::commonutil::randomRange(-32, 32);
            shape.m_y3 = shape.m_y1 + geometrize::commonutil::randomRange(-32, 32);
        });

        mutator.setMutatorFunction([this](geometrize::Circle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
            switch(r) {
                case 0:
                {
                    shape.m_x = geometrize::commonutil::clamp(shape.m_x + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y = geometrize::commonutil::clamp(shape.m_y + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
                case 1:
                {
                    shape.m_r = geometrize::commonutil::clamp(shape.m_r + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
                    break;
                }
            }
        });

        mutator.setMutatorFunction([this](geometrize::Ellipse& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
            switch(r) {
                case 0:
                {
                    shape.m_x = geometrize::commonutil::clamp(shape.m_x + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y = geometrize::commonutil::clamp(shape.m_y + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
                case 1:
                {
                    shape.m_rx = geometrize::commonutil::clamp(shape.m_rx + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
                    break;
                }
                case 2:
                {
                    shape.m_ry = geometrize::commonutil::clamp(shape.m_ry + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
                    break;
                }
            }
        });

        mutator.setMutatorFunction([this](geometrize::Line& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};

            switch(r) {
                case 0:
                {
                    shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
                case 1:
                {
                    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
            }
        });
        mutator
                .setMutatorFunction([this](geometrize::Polyline& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};
            const std::int32_t i{geometrize::commonutil::randomRange(static_cast<std::size_t>(0), shape.m_points.size() - 1)};

            std::pair<std::int32_t, std::int32_t> point{shape.m_points[i]};
            point.first = geometrize::commonutil::clamp(point.first + geometrize::commonutil::randomRange(-64, 64), 0, xBound - 1);
            point.second = geometrize::commonutil::clamp(point.second + geometrize::commonutil::randomRange(-64, 64), 0, yBound - 1);

            shape.m_points[i] = point;
        });

        mutator.setMutatorFunction([this](geometrize::QuadraticBezier& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
            switch(r) {
                case 0:
                {
                    shape.m_cx = geometrize::commonutil::clamp(shape.m_cx + geometrize::commonutil::randomRange(-8, 8), 0, xBound - 1);
                    shape.m_cy = geometrize::commonutil::clamp(shape.m_cy + geometrize::commonutil::randomRange(-8, 8), 0, yBound - 1);
                    break;
                }
                case 1:
                {
                    shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
                    shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
                    break;
                }
                case 2:
                {
                    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
                    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
                    break;
                }
            }
        });

        mutator.setMutatorFunction([this](geometrize::Rectangle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
            switch(r) {
                case 0:
                {
                    shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
                case 1:
                {
                    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
            }
        });

        mutator.setMutatorFunction([this](geometrize::RotatedEllipse& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 3)};
            switch(r) {
                case 0:
                {
                    shape.m_x = geometrize::commonutil::clamp(shape.m_x + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
                    shape.m_y = geometrize::commonutil::clamp(shape.m_y + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
                    break;
                }
                case 1:
                {
                    shape.m_rx = geometrize::commonutil::clamp(shape.m_rx + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
                    break;
                }
                case 2:
                {
                    shape.m_ry = geometrize::commonutil::clamp(shape.m_ry + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
                    break;
                }
                case 3:
                {
                    shape.m_angle = geometrize::commonutil::clamp(shape.m_angle + geometrize::commonutil::randomRange(-16, 16), 0, 360);
                    break;
                }
            }
        });

        mutator.setMutatorFunction([this](geometrize::RotatedRectangle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
            switch(r) {
                case 0:
                {
                    shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
                    shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
                    break;
                }
                case 1:
                {
                    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
                    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
                    break;
                }
                case 2:
                {
                    shape.m_angle = geometrize::commonutil::clamp(shape.m_angle + geometrize::commonutil::randomRange(-4, 4), 0, 360);
                    break;
                }
            }
        });

        mutator.setMutatorFunction([this](geometrize::Triangle& shape) {
            const std::int32_t xBound{shape.m_model.getWidth()};
            const std::int32_t yBound{shape.m_model.getHeight()};

            const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
            switch(r) {
                case 0:
                {
                    shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
                    shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
                    break;
                }
                case 1:
                {
                    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
                    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
                    break;
                }
                case 2:
                {
                    shape.m_x3 = geometrize::commonutil::clamp(shape.m_x3 + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
                    shape.m_y3 = geometrize::commonutil::clamp(shape.m_y3 + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
                    break;
                }
            }
        });
    }

    std::unique_ptr<chaiscript::ChaiScript> m_engine;
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
