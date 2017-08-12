# This is the project file for the Universal Windows Platform version of Geometrize.

CONFIG += windeployqt

# Run the regular Geometrize build
include(geometrize.pro)

# TODO
WINRT_MANIFEST.background = white
WINRT_MANIFEST.description = Geometrize, the app for geometrizing images into geometric primitives
WINRT_MANIFEST.foreground = light
#WINRT_MANIFEST.iconic_tile_icon = uwp/iconic_tile_icon.png
#WINRT_MANIFEST.iconic_tile_small = uwp/iconic_tile_small.png
WINRT_MANIFEST.logo_30x30 = uwp/logo_30x30.png
WINRT_MANIFEST.logo_41x41 = uwp/logo_41x41.png
WINRT_MANIFEST.logo_70x70 = uwp/logo_70x70.png
WINRT_MANIFEST.logo_71x71 = uwp/logo_71x71.png
WINRT_MANIFEST.logo_150x150 = uwp/logo_150x150.png
WINRT_MANIFEST.logo_310x150 = uwp/logo_310x150.png
WINRT_MANIFEST.logo_310x310 = uwp/logo_310x310.png
WINRT_MANIFEST.logo_620x300 = uwp/logo_620x300.png
WINRT_MANIFEST.logo_480x800 = uwp/logo_480x800.png
#WINRT_MANIFEST.logo_large = uwp/logo_large.png
#WINRT_MANIFEST.logo_medium = uwp/logo_medium.png
#WINRT_MANIFEST.logo_small = uwp/logo_small.png
#WINRT_MANIFEST.logo_splash = uwp/logo_splash.png
#WINRT_MANIFEST.logo_store = uwp/logo_store.png
#WINRT_MANIFEST.logo_wide = uwp/logo_wide.png
WINRT_MANIFEST.name = Geometrize
WINRT_MANIFEST.publisher = Sam Twidale