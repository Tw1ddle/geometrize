#pragma once

#include <vector>

#include <QObject>

#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace task
{

/**
 * @brief The ShapeCollection class is a wrapper for the vector of shapes that have been added to the image task so far.
 */
class ShapeCollection : public QObject
{
    Q_OBJECT
public:
    ShapeCollection() = default;
    ShapeCollection& operator=(const ShapeCollection&) = default;
    ShapeCollection(const ShapeCollection&) = default;
    ~ShapeCollection() = default;

    bool empty() const;

    std::size_t size() const;

    void clear();

    const std::vector<geometrize::ShapeResult>& getShapeVector() const;

    void appendShapes(const std::vector<geometrize::ShapeResult>& shapes);

    geometrize::ShapeResult& back();

signals:
    void signal_sizeChanged(std::size_t to);
    void signal_appendedShapes(const std::vector<geometrize::ShapeResult>&);

private:
    std::vector<geometrize::ShapeResult> m_shapes; ///> The shapes and score results created by the image task
};

}
}
