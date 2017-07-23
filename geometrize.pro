QT += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Geometrize
TEMPLATE = app

# Flag for ChaiScript linking
msvc:QMAKE_CXXFLAGS += -bigobj

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

# Include resources that Geometrize needs, such as icons, images and scripts
include($$PWD/resources/resources.pri)
