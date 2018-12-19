#pragma once

#ifdef DATASLINGER_INCLUDED // Not enabled by default - this is an optional extra library

namespace geometrize
{

/**
 * @brief setupImageSlinger Sets up a WebSocket server for sending geometrized images out to clients e.g. other open source applications with a client hacked in.
 * Searches for config files "imageslinger_key_bindings.json" and "imageslinger_connection_options" for global keyboard shortcuts and the connection options to use.
 */
void setupImageSlinger();

}

#endif
