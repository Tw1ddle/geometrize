#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

namespace geometrize
{

namespace constants
{

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
        return tr("Geometrize", "This means to 'turn into geometry' or to 'represent using geometric primitives'. The name of the application, so must be short and meaningful - do not translate if this is difficult.");
    }

    /**
     * @brief getApplicationDescription Gets a short description of the application.
     * @return Short description of the application.
     */
    static QString getApplicationDescription()
    {
        return tr("A tool for reproducing images as animated geometric primitives.", "This is a short description of what this application does.");
    }

    /**
     * @brief getCopyrightNotice Gets a copyright notice for the application.
     * @return The copyright notice for the application.
     */
    static QString getCopyrightNotice()
    {
        return tr("Copyright 2015-2016 Sam Twidale", "This is the copyright notice of the application.");
    }
};

const QString DEVELOPER_WEBSITE_URL{"http://www.samcodes.co.uk"};
const QString TECHNICAL_SUPPORT_URL{"http://www.samcodes.co.uk/contact"};
const QString VIDEO_TUTORIAL_URL{"https://www.youtube.com/playlist?list=PLe9ogi_J4cFgcqLdpmPC7GdFV5ohJPEzN"};

}

}

#endif // CONSTANTS_H
