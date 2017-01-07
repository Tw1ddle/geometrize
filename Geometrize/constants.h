#pragma once

#include <QObject>

namespace geometrize
{

namespace constants
{

/**
 * @brief The Strings class contains methods for getting miscelleanous translatable strings.
 */
class Strings : public QObject
{
public:
    Strings() = delete;
    Strings& operator=(const Strings&) = delete;
    Strings(const Strings&) = delete;

    /**
     * @brief getApplicationName Gets the name of the application.
     * @return The name of the application.
     */
    static QString getApplicationName()
    {
        return tr("Geometrize", "This means to 'turn into geometry' or to 'represent using geometric primitives'. The name of the application, so must be short and meaningful. Do not translate if this is difficult to communicate in one word.");
    }

    /**
     * @brief getApplicationDescription Gets a short description of the application.
     * @return Short description of the application.
     */
    static QString getApplicationDescription()
    {
        return tr("A tool for reproducing media as animated geometric primitives.", "This is a short description of what this application does.");
    }

    /**
     * @brief getCopyrightNotice Gets a copyright notice for the application.
     * @return The copyright notice for the application.
     */
    static QString getCopyrightNotice()
    {
        return tr("Copyright 2015-2017 Sam Twidale", "This is the copyright notice of the application.");
    }
};

const QString PROJECT_WEBSITE_URL{"http://www.geometrize.it"}; ///< The project's dedicated website URL.
const QString DEVELOPER_WEBSITE_URL{"http://www.samcodes.co.uk"}; ///< The core developer's personal website URL.
const QString TECHNICAL_SUPPORT_URL{"http://www.samcodes.co.uk/contact"}; ///< The technical support webpage URL for the software.
const QString MORE_RESOURCES_URL{"https://github.com/Tw1ddle/geometrize-resources"}; ///< Webpage URL to a page providing additional templates, scripts and/or helpful links.
const QString VIDEO_TUTORIAL_URL{"https://www.youtube.com/playlist?list=PLe9ogi_J4cFgcqLdpmPC7GdFV5ohJPEzN"}; ///< The video tutorial webpage URL for the software.

}

}
