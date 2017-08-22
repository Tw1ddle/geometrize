QT += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Geometrize
TEMPLATE = app

# Enable C++11 features
CONFIG += c++11

# Disable assertions in release builds (for gcc, clang etc builds)
CONFIG(release, debug|release): DEFINES += NDEBUG

# Flag for ChaiScript linking
# Note that mingw32 builds may not work with the compiled-in stdlib because of a "too many sections error" with Chaiscript
msvc:QMAKE_CXXFLAGS += -bigobj

# Options for the Universal Windows Platform version of Geometrize
winrt|wince {
    include($$PWD/uwp/geometrize_uwp.pri)
}

# Include library dependencies
INCLUDEPATH += $$PWD/lib/cereal/include \
    $$PWD/lib/chaiscript/include
include($$PWD/lib/geometrize/geometrize/geometrize.pri)
include($$PWD/lib/qt_google_analytics/qt-google-analytics.pri)
include($$PWD/lib/qtgifimage/src/gifimage/qtgifimage.pri)

# Include the Geometrize headers, sources, UI forms
include($$PWD/geometrize/sources.pri)

# Generate localization TS files and then binary QM files, which it puts into the resources folder
include($$PWD/translations/translations.pri)

# Include resources that Geometrize requires, such as icons, images and scripts
# Note this must come last, as resource files are automatically generated
include($$PWD/resources/resources.pri)
