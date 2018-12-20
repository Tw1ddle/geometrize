#pragma once

#ifdef DATASLINGER_INCLUDED

// Note this stuff is not enabled by default - this is optional extra functionality for sending/receiving images and other data via WebSockets
// Note none of this stuff is localized, that's deliberate

#include <QImage>

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
 * @brief installImageSlingerKeyboardShortcut Installs the keyboard shortcuts for sending images etc. Note use of the shortcuts requires the slinger/receiver to be functional.
 * Searches for config file "imageslinger_misc_options.json" for keyboard shortcuts to use.
 * @param widget The widget to install the keyboard shortcuts upon.
 * @param imageTask The image task associated with the widget.
 */
void installImageSlingerKeyboardShortcuts(geometrize::dialog::ImageTaskWindow* widget, geometrize::task::ImageTask* imageTask);

}

#endif
