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
    mainwindow.cpp \
    imagejobcontext.cpp \
    geometrizer/geometrizer/bitmap/pixelformat.cpp \
    geometrizer/geometrizer/bitmap/rgba.cpp \
    geometrizer/geometrizer/exporters/shapearrayexporter.cpp \
    geometrizer/geometrizer/exporters/shapejsonexporter.cpp \
    geometrizer/geometrizer/runner/imagerunner.cpp \
    geometrizer/geometrizer/shape/circle.cpp \
    geometrizer/geometrizer/shape/ellipse.cpp \
    geometrizer/geometrizer/shape/rectangle.cpp \
    geometrizer/geometrizer/shape/rotatedellipse.cpp \
    geometrizer/geometrizer/shape/rotatedrectangle.cpp \
    geometrizer/geometrizer/shape/shapefactory.cpp \
    geometrizer/geometrizer/shape/triangle.cpp \
    geometrizer/geometrizer/geometrize.cpp \
    geometrizer/geometrizer/model.cpp \
    geometrizer/geometrizer/scanline.cpp \
    geometrizer/geometrizer/state.cpp \
    geometrizer/geometrizer/util.cpp \
    geometrizer/unit/bitmap/pixelformat.cpp \
    geometrizer/unit/bitmap/rgba.cpp \
    geometrizer/unit/exporters/shapearrayexporter.cpp \
    geometrizer/unit/exporters/shapejsonexporter.cpp \
    geometrizer/unit/runner/imagerunner.cpp \
    geometrizer/unit/shape/circle.cpp \
    geometrizer/unit/shape/ellipse.cpp \
    geometrizer/unit/shape/rectangle.cpp \
    geometrizer/unit/shape/rotatedellipse.cpp \
    geometrizer/unit/shape/rotatedrectangle.cpp \
    geometrizer/unit/shape/shapefactory.cpp \
    geometrizer/unit/shape/triangle.cpp \
    geometrizer/unit/geometrize.cpp \
    geometrizer/unit/model.cpp \
    geometrizer/unit/scanline.cpp \
    geometrizer/unit/state.cpp \
    geometrizer/unit/util.cpp

HEADERS  += mainwindow.h \
    imagejobcontext.h \
    geometrizer/geometrizer/bitmap/bitmapdata.h \
    geometrizer/geometrizer/bitmap/pixelformat.h \
    geometrizer/geometrizer/bitmap/rgba.h \
    geometrizer/geometrizer/exporters/shapearrayexporter.h \
    geometrizer/geometrizer/exporters/shapejsonexporter.h \
    geometrizer/geometrizer/runner/imagerunner.h \
    geometrizer/geometrizer/shape/circle.h \
    geometrizer/geometrizer/shape/ellipse.h \
    geometrizer/geometrizer/shape/rectangle.h \
    geometrizer/geometrizer/shape/rotatedellipse.h \
    geometrizer/geometrizer/shape/rotatedrectangle.h \
    geometrizer/geometrizer/shape/shape.h \
    geometrizer/geometrizer/shape/shapefactory.h \
    geometrizer/geometrizer/shape/shapetype.h \
    geometrizer/geometrizer/shape/triangle.h \
    geometrizer/geometrizer/geometrize.h \
    geometrizer/geometrizer/model.h \
    geometrizer/geometrizer/scanline.h \
    geometrizer/geometrizer/state.h \
    geometrizer/geometrizer/util.h \
    geometrizer/unit/bitmap/bitmap.h \
    geometrizer/unit/bitmap/pixelformat.h \
    geometrizer/unit/bitmap/rgba.h \
    geometrizer/unit/exporters/shapearrayexporter.h \
    geometrizer/unit/exporters/shapejsonexporter.h \
    geometrizer/unit/runner/imagerunner.h \
    geometrizer/unit/shape/circle.h \
    geometrizer/unit/shape/ellipse.h \
    geometrizer/unit/shape/rectangle.h \
    geometrizer/unit/shape/rotatedellipse.h \
    geometrizer/unit/shape/rotatedrectangle.h \
    geometrizer/unit/shape/shape.h \
    geometrizer/unit/shape/shapefactory.h \
    geometrizer/unit/shape/shapetype.h \
    geometrizer/unit/shape/triangle.h \
    geometrizer/unit/catch.hpp \
    geometrizer/unit/geometrize.h \
    geometrizer/unit/model.h \
    geometrizer/unit/scanline.h \
    geometrizer/unit/state.h \
    geometrizer/unit/util.h

FORMS    += forms/mainwindow.ui
