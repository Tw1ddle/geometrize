#pragma once

#ifdef DATASLINGER_INCLUDED

// Note this stuff is not enabled by default - this is optional extra functionality for sending/receiving images and other data via WebSockets
// Note none of this stuff is localized, that's deliberate

#include <QImage>

#include <string>

namespace geometrize
{
namespace dialog
{

class ImageTaskWindow;

}

namespace task
{

class ImageTask;

}
}

namespace geometrize
{

/**
 * @brief setupImageSlinger Sets up a WebSocket server for sending geometrized images out to clients e.g. other open source applications with a client hacked in.
 * Searches for config file "imageslinger_connection_options.json" for the connection options to use.
 */
void setupImageSlinger();

/**
 * @brief setupImageReceiver Sets up a WebSocket client for receiving geometrized images e.g. web applications, open source applications with a client hacked in.
 * Searches for config file "imagereceiver_connection_options.json" for connection options to use.
 */
void setupImageReceiver();

/**
 * @brief insertImageSlingerUserInterface Inserts UI and keyboard shortcuts for sending images etc.
 * Note use of the keyboard shortcuts requires the slinger/receiver to be functional.
 * Searches for config file "imageslinger_misc_options.json" for keyboard shortcuts to use.
 * @param widget The widget to install the additional UI and keyboard shortcuts upon.
 * @param imageTask The image task associated with the widget.
 */
void installImageSlingerUserInterface(geometrize::dialog::ImageTaskWindow* widget);

/**
 * @brief setupShapeSvgSlinger Sets up a WebSocket server for sending individual SVG shapes out to clients e.g. other open source applications with a client hacked in.
 * Searches for config file "svgshapeslinger_connection_options.json" for the connection options to use.
 */
void setupSvgShapeSlinger();

/**
 * @brief sendSvgShapeData Sends the given SVG shape data as a string out to clients.
 * @param s The SVG shape data for a shape.
 */
void sendSvgShapeData(const std::string& s);

}

#endif
