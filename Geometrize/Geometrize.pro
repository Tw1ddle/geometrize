#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T19:04:04
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Geometrize
TEMPLATE = app

RC_ICONS = icon.ico
RC_FILE = winicon.rc
RESOURCES = resources/application_resources.qrc

QMAKE_CXXFLAGS += -bigobj

INCLUDEPATH += $$PWD/../lib/cereal/include \
    $$PWD/../lib/chaiscript/include

include($$PWD/../lib/geometrize/geometrize/geometrize.pri)
include($$PWD/../lib/qt_google_analytics/qt-google-analytics.pri)

SOURCES += commandlineparser.cpp \
    formatsupport.cpp \
    main.cpp \
    recentitem.cpp \
    recentitems.cpp \
    runguard.cpp \
    searchpaths.cpp \
    templatemanifest.cpp \
    versioninfo.cpp \
    util.cpp \
    analytics/analytics.cpp \
    analytics/analyticswrapper.cpp \
    common/sharedapp.cpp \
    common/uiactions.cpp \
    dialog/aboutdialog.cpp \
    dialog/commandlineedit.cpp \
    dialog/creditsdialog.cpp \
    dialog/elidedlabel.cpp \
    dialog/globalpreferencestabdialog.cpp \
    dialog/imagejobwindow.cpp \
    dialog/launchwindow.cpp \
    dialog/licensedialog.cpp \
    dialog/openurldialog.cpp \
    dialog/quitdialog.cpp \
    dialog/recentitemwidget.cpp \
    dialog/recentjobslist.cpp \
    dialog/runscriptdialog.cpp \
    dialog/scriptconsole.cpp \
    dialog/searchbox.cpp \
    dialog/templatebutton.cpp \
    dialog/templategrid.cpp \
    image/imageloader.cpp \
    image/imagelookup.cpp \
    job/imagejob.cpp \
    job/imagejobcreator.cpp \
    job/joblookup.cpp \
    job/jobutil.cpp \
    layout/flowlayout.cpp \
    network/completionhandlers.cpp \
    network/downloader.cpp \
    network/networkactions.cpp \
    preferences/globalpreferences.cpp \
    preferences/imagejobpreferences.cpp \
    script/bindingscreator.cpp \
    script/bindingswrapper.cpp \
    script/chaiscriptcreator.cpp \
    script/scriptoptions.cpp \
    script/scriptrunner.cpp \
    script/scriptutil.cpp \
    serialization/serializationutil.cpp \
    dialog/imagegraphicsview.cpp \
    dialog/collapsiblesection.cpp

HEADERS += commandlineparser.h \
    constants.h \
    formatsupport.h \
    recentitem.h \
    recentitems.h \
    runguard.h \
    searchpaths.h \
    templatemanifest.h \
    util.h \
    versioninfo.h \
    winversioninfo.h \
    analytics/analytics.h \
    analytics/analyticswrapper.h \
    common/sharedapp.h \
    common/uiactions.h \
    dialog/aboutdialog.h \
    dialog/commandlineedit.h \
    dialog/creditsdialog.h \
    dialog/elidedlabel.h \
    dialog/globalpreferencestabdialog.h \
    dialog/imagejobwindow.h \
    dialog/launchwindow.h \
    dialog/licensedialog.h \
    dialog/quitdialog.h \
    dialog/recentitemwidget.h \
    dialog/recentjobslist.h \
    dialog/openurldialog.h \
    dialog/scriptconsole.h \
    dialog/searchbox.h \
    dialog/runscriptdialog.h \
    dialog/templatebutton.h \
    dialog/templategrid.h \
    image/imageloader.h \
    image/imagelookup.h \
    job/imagejob.h \
    job/imagejobcreator.h \
    job/joblookup.h \
    job/jobutil.h \
    layout/flowlayout.h \
    network/completionhandlers.h \
    network/downloader.h \
    network/networkactions.h \
    preferences/globalpreferences.h \
    preferences/imagejobpreferences.h \
    script/bindingscreator.h \
    script/bindingswrapper.h \
    script/chaiscriptcreator.h \
    script/scriptoptions.h \
    script/scriptrunner.h \
    script/scriptutil.h \
    serialization/globalpreferencesdata.h \
    serialization/imagejobpreferencesdata.h \
    serialization/templatemetadata.h \
    serialization/serializationutil.h \
    serialization/stringvectordata.h \
    dialog/imagegraphicsview.h \
    dialog/collapsiblesection.h

FORMS += dialog/aboutdialog.ui \
    dialog/commandlineedit.ui \
    dialog/creditsdialog.ui \
    dialog/globalpreferencestabdialog.ui \
    dialog/imagejobwindow.ui \
    dialog/launchwindow.ui \
    dialog/licensedialog.ui \
    dialog/openurldialog.ui \
    dialog/quitdialog.ui \
    dialog/recentitemwidget.ui \
    dialog/runscriptdialog.ui \
    dialog/scriptconsole.ui \
    dialog/templatebutton.ui
