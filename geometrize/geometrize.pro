QT += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Geometrize
TEMPLATE = app

msvc:QMAKE_CXXFLAGS += -bigobj

INCLUDEPATH += $$PWD/../lib/cereal/include \
    $$PWD/../lib/chaiscript/include

include($$PWD/../lib/geometrize/geometrize/geometrize.pri)
include($$PWD/../lib/qt_google_analytics/qt-google-analytics.pri)
include($$PWD/../lib/qtgifimage/src/gifimage/qtgifimage.pri)

SOURCES += commandlineparser.cpp \
    formatsupport.cpp \
    localization.cpp \
    logger.cpp \
    logmessageevents.cpp \
    logmessagehandlers.cpp \
    main.cpp \
    recentitem.cpp \
    recentitems.cpp \
    runguard.cpp \
    searchpaths.cpp \
    strings.cpp \
    templatemanifest.cpp \
    versioninfo.cpp \
    util.cpp \
    analytics/analytics.cpp \
    analytics/analyticswrapper.cpp \
    common/sharedapp.cpp \
    common/uiactions.cpp \
    dialog/aboutdialog.cpp \
    dialog/collapsiblepanel.cpp \
    dialog/commandlineedit.cpp \
    dialog/completionbox.cpp \
    dialog/creditsdialog.cpp \
    dialog/elidedlabel.cpp \
    dialog/globalpreferencesdialog.cpp \
    dialog/imagegraphicsview.cpp \
    dialog/imagejobpixmapgraphicsitem.cpp \
    dialog/imagejobscene.cpp \
    dialog/imagejobwindow.cpp \
    dialog/languageselectwidget.cpp \
    dialog/languageselectlist.cpp \
    dialog/launchwindow.cpp \
    dialog/licensedialog.cpp \
    dialog/openurldialog.cpp \
    dialog/recentitemwidget.cpp \
    dialog/recentjobslist.cpp \
    dialog/runscriptdialog.cpp \
    dialog/scriptconsole.cpp \
    dialog/scripteditorwidget.cpp \
    dialog/svgpreviewdialog.cpp \
    dialog/svgpreviewview.cpp \
    dialog/templatebutton.cpp \
    dialog/templategrid.cpp \
    exporter/canvasanimationexporter.cpp \
    exporter/gifexporter.cpp \
    exporter/imageexporter.cpp \
    exporter/shapedataexporter.cpp \
    exporter/webglanimationexporter.cpp \
    image/imageloader.cpp \
    image/imagelookup.cpp \
    job/imagejob.cpp \
    job/imagejobcreator.cpp \
    job/imagejobworker.cpp \
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
    script/shapemutationrules.cpp \
    serialization/serializationutil.cpp \
    serialization/streamview.cpp

HEADERS += commandlineparser.h \
    constants.h \
    formatsupport.h \
    localization.h \
    logger.h \
    logmessageevents.h \
    logmessagehandlers.h \
    recentitem.h \
    recentitems.h \
    runguard.h \
    searchpaths.h \
    strings.h \
    templatemanifest.h \
    util.h \
    versioninfo.h \
    analytics/analytics.h \
    analytics/analyticswrapper.h \
    common/sharedapp.h \
    common/uiactions.h \
    dialog/aboutdialog.h \
    dialog/collapsiblepanel.h \
    dialog/commandlineedit.h \
    dialog/completionbox.h \
    dialog/creditsdialog.h \
    dialog/elidedlabel.h \
    dialog/globalpreferencesdialog.h \
    dialog/imagegraphicsview.h \
    dialog/imagejobpixmapgraphicsitem.h \
    dialog/imagejobscene.h \
    dialog/imagejobwindow.h \
    dialog/languageselectlist.h \
    dialog/languageselectwidget.h \
    dialog/launchwindow.h \
    dialog/licensedialog.h \
    dialog/openurldialog.h \
    dialog/recentjobslist.h \
    dialog/recentitemwidget.h \
    dialog/scriptconsole.h \
    dialog/scripteditorwidget.h \
    dialog/svgpreviewdialog.h \
    dialog/svgpreviewview.h \
    dialog/runscriptdialog.h \
    dialog/templatebutton.h \
    dialog/templategrid.h \
    exporter/canvasanimationexporter.h \
    exporter/gifexporter.h \
    exporter/imageexporter.h \
    exporter/shapedataexporter.h \
    exporter/webglanimationexporter.h \
    image/imageloader.h \
    image/imagelookup.h \
    job/imagejob.h \
    job/imagejobcreator.h \
    job/imagejobworker.h \
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
    script/chaiscriptmathextras.h \
    script/scriptoptions.h \
    script/scriptrunner.h \
    script/scriptutil.h \
    script/shapemutationrules.h \
    serialization/globalpreferencesdata.h \
    serialization/imagejobpreferencesdata.h \
    serialization/serializationutil.h \
    serialization/stringvectordata.h \
    serialization/templatemetadata.h \
    serialization/streamview.h

FORMS += dialog/aboutdialog.ui \
    dialog/commandlineedit.ui \
    dialog/creditsdialog.ui \
    dialog/globalpreferencesdialog.ui \
    dialog/imagejobwindow.ui \
    dialog/languageselectwidget.ui \
    dialog/launchwindow.ui \
    dialog/licensedialog.ui \
    dialog/openurldialog.ui \
    dialog/recentitemwidget.ui \
    dialog/runscriptdialog.ui \
    dialog/scriptconsole.ui \
    dialog/scripteditorwidget.ui \
    dialog/svgpreviewdialog.ui \
    dialog/templatebutton.ui

# Generates localization TS files and then binary QM files, which it puts into the resources folder
include($$PWD/../translations/translations.pri)

RESOURCES += ../resources/icons.qrc \
    ../resources/licenses.qrc \
    ../resources/misc.qrc \
    ../resources/scripts.qrc \
    ../resources/settings.qrc \
    ../resources/templates.qrc \
    ../resources/translations.qrc \
    ../resources/web_export.qrc

RC_ICONS = ../app_icons/icon.ico
RC_FILE = ../app_icons/winicon.rc
