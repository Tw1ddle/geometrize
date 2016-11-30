TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../main.cpp

HEADERS += ../catch.hpp \
    ../tests/bitmap/bitmapdata.h \
    ../tests/bitmap/rgba.h \
    ../tests/exporters/shapearrayexporter.h \
    ../tests/exporters/shapejsonexporter.h \
    ../tests/runner/imagerunner.h \
    ../tests/shape/circle.h \
    ../tests/shape/ellipse.h \
    ../tests/shape/rectangle.h \
    ../tests/shape/rotatedellipse.h \
    ../tests/shape/rotatedrectangle.h \
    ../tests/shape/shape.h \
    ../tests/shape/shapefactory.h \
    ../tests/shape/shapetype.h \
    ../tests/shape/triangle.h \
    ../tests/geometrize.h \
    ../tests/model.h \
    ../tests/scanline.h \
    ../tests/state.h \
    ../tests/util.h
