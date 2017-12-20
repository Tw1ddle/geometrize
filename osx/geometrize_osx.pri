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

# Copy over placeholder Localizable.strings files for the languages Geometrize supports
# Note this could be automated but since the language codes we use might not be quite 1-1 with
# what the appstore wants (e.g. pt_PT and pt_BR) this is done manually for now
MAC_LANGUAGE_CODES = ar bg cs da de el en en_AU en_CA en_GB en_US es es_ES es_MX fi fr fr_CA fr_FR id it ja ko ms nl no pl pt pt_BR pt_PT ru sk sr sv th tr vi zh zh_Hans zh_Hant

for(LANGUAGE_CODE, MAC_LANGUAGE_CODES) {
    MAC_LANGUAGE_DATA_$${LANGUAGE_CODE}.files = $$PWD/Localizable.strings
    MAC_LANGUAGE_DATA_$${LANGUAGE_CODE}.path = "Contents/Resources/$${LANGUAGE_CODE}.lproj"
    QMAKE_BUNDLE_DATA += MAC_LANGUAGE_DATA_$${LANGUAGE_CODE}
}
