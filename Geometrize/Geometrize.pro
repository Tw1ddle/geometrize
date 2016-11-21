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

SOURCES += main.cpp\
    versioninfo.cpp \
    imagejobcontext.cpp \
    preferences.cpp \
    imagejobpreferences.cpp \
    lib/geometrizer/geometrizer/bitmap/pixelformat.cpp \
    lib/geometrizer/geometrizer/bitmap/rgba.cpp \
    lib/geometrizer/geometrizer/exporters/shapearrayexporter.cpp \
    lib/geometrizer/geometrizer/exporters/shapejsonexporter.cpp \
    lib/geometrizer/geometrizer/runner/imagerunner.cpp \
    lib/geometrizer/geometrizer/shape/circle.cpp \
    lib/geometrizer/geometrizer/shape/ellipse.cpp \
    lib/geometrizer/geometrizer/shape/rectangle.cpp \
    lib/geometrizer/geometrizer/shape/rotatedellipse.cpp \
    lib/geometrizer/geometrizer/shape/rotatedrectangle.cpp \
    lib/geometrizer/geometrizer/shape/shapefactory.cpp \
    lib/geometrizer/geometrizer/shape/triangle.cpp \
    lib/geometrizer/geometrizer/geometrize.cpp \
    lib/geometrizer/geometrizer/model.cpp \
    lib/geometrizer/geometrizer/scanline.cpp \
    lib/geometrizer/geometrizer/state.cpp \
    lib/geometrizer/geometrizer/util.cpp \
    forms/aboutdialog.cpp \
    forms/mainwindow.cpp \
    forms/quitdialog.cpp

HEADERS  += versioninfo.h \
    imagejobcontext.h \
    preferences.h \
    imagejobpreferences.h \
    constants.h \
    lib/geometrizer/geometrizer/bitmap/bitmapdata.h \
    lib/geometrizer/geometrizer/bitmap/pixelformat.h \
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
    lib/geometrizer/geometrizer/shape/shapetype.h \
    lib/geometrizer/geometrizer/shape/triangle.h \
    lib/geometrizer/geometrizer/geometrize.h \
    lib/geometrizer/geometrizer/model.h \
    lib/geometrizer/geometrizer/scanline.h \
    lib/geometrizer/geometrizer/state.h \
    lib/geometrizer/geometrizer/util.h \
    forms/aboutdialog.h \
    forms/mainwindow.h \
    forms/quitdialog.h \

FORMS    += forms/mainwindow.ui \
    forms/aboutdialog.ui \
    forms/quitdialog.ui \
    forms/globalpreferencesdialog.ui
