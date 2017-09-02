INCLUDEPATH += $$PWD

HEADERS += $$files($$PWD/*.h, true)
SOURCES += $$files($$PWD/*.cpp, true)
FORMS += $$files($$PWD/dialog/*.ui, true)
