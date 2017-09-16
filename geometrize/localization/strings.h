#pragma once

#include <QObject>
#include <QString>

#include "geometrize/shape/shapetypes.h"

namespace geometrize
{

namespace strings
{

/**
 * @brief The Strings class contains methods for getting miscelleanous translatable strings.
 */
class Strings : public QObject
{
    Q_OBJECT
public:
    Strings() = delete;
    Strings& operator=(const Strings&) = delete;
    Strings(const Strings&) = delete;

    /**
     * @brief getApplicationName Gets the name of the application.
     * @return The name of the application.
     */
    static QString getApplicationName();

    /**
     * @brief getApplicationDescription Gets a short description of the application.
     * @return Short description of the application.
     */
    static QString getShortApplicationDescription();

    /**
     * @brief getCopyrightNotice Gets a copyright notice for the application.
     * @return The copyright notice for the application.
     */
    static QString getCopyrightNotice();

    /**
     * @brief getShapeTypeNamePlural Gets the plural name of the shape for the given shape type.
     * @param shapeType The type of shape whose name will be returned.
     * @return The name of the shape for the given shape type e.g. RECTANGLE type returns "Rectangles".
     */
    static QString getShapeTypeNamePlural(geometrize::ShapeTypes shapeType);
};

}

}
