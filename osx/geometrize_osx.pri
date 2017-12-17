OTHER_FILES += $$PWD/*.*

# This makes Geometrize use a special license for the Mac App Store (because I think their DRM is incompatible with the GPL)
DEFINES += MAC_APPSTORE_LICENSE

# Include a custom plist, with details like the languages the app has been translated into
QMAKE_INFO_PLIST = $$PWD/GeometrizeAppInfo.plist

ICON = $$PWD/icon.icns

# Force Qt to copy over the Mac icon file
RESOURCE_FILES.files = $$ICON
RESOURCE_FILES.path = Contents/Resources
QMAKE_BUNDLE_DATA += RESOURCE_FILES