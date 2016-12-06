#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T19:04:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Geometrize
TEMPLATE = app

RC_ICONS = icon.ico
RC_FILE = winicon.rc
RESOURCES = resources/application_resources.qrc

SOURCES += main.cpp \
    versioninfo.cpp \
    imagejobcontext.cpp \
    preferences.cpp \
    imagejobpreferences.cpp \
    sharedapp.cpp \
    recentfiles.cpp \
    dialog/aboutdialog.cpp \
    dialog/quitdialog.cpp \
    dialog/licensedialog.cpp \
    dialog/creditsdialog.cpp \
    dialog/preferencestabdialog.cpp \
    dialog/imagejobwindow.cpp \
    dialog/launchwindow.cpp

HEADERS  += versioninfo.h \
    winversioninfo.h \
    imagejobcontext.h \
    preferences.h \
    imagejobpreferences.h \
    constants.h \
    sharedapp.h \
    recentfiles.h \
    lib/geometrizer/geometrizer/bitmap/bitmapdata.h \
    lib/geometrizer/geometrizer/bitmap/rgba.h \
    lib/geometrizer/geometrizer/exporters/shapearrayexporter.h \
    lib/geometrizer/geometrizer/exporters/shapejsonexporter.h \
    lib/geometrizer/geometrizer/runner/imagerunner.h \
    lib/geometrizer/geometrizer/shape/circle.h \
    lib/geometrizer/geometrizer/shape/ellipse.h \
    lib/geometrizer/geometrizer/shape/rectangle.h \
    lib/geometrizer/geometrizer/shape/rotatedellipse.h \
    lib/geometrizer/geometrizer/shape/rotatedrectangle.h \
    lib/geometrizer/geometrizer/shape/shape.h \
    lib/geometrizer/geometrizer/shape/shapefactory.h \
    lib/geometrizer/geometrizer/shape/shapetypes.h \
    lib/geometrizer/geometrizer/shape/triangle.h \
    lib/geometrizer/geometrizer/geometrize.h \
    lib/geometrizer/geometrizer/model.h \
    lib/geometrizer/geometrizer/scanline.h \
    lib/geometrizer/geometrizer/state.h \
    lib/geometrizer/geometrizer/util.h \
    dialog/aboutdialog.h \
    dialog/quitdialog.h \
    dialog/licensedialog.h \
    dialog/creditsdialog.h \
    dialog/preferencestabdialog.h \
    dialog/imagejobwindow.h \
    dialog/launchwindow.h

FORMS    += dialog/imagejobwindow.ui \
    dialog/aboutdialog.ui \
    dialog/quitdialog.ui \
    dialog/globalpreferencesdialog.ui \
    dialog/licensedialog.ui \
    dialog/creditsdialog.ui \
    dialog/preferencestabdialog.ui \
    dialog/launchwindow.ui
