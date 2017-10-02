# Options for the Universal Windows Platform versions of Geometrize

CONFIG += windeployqt

OTHER_FILES += $$PWD/*.*

# Configure the UWP package manifest
WINRT_MANIFEST.background = "black"
WINRT_MANIFEST.description = "Geometrize, the app for geometrizing images into geometric primitives"
WINRT_MANIFEST.foreground = "light"
WINRT_MANIFEST.name = "Geometrize"
WINRT_MANIFEST.publisher = "Sam Twidale"
