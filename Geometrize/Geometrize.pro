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

SOURCES += main.cpp \
    commandlineparser.cpp \
    formatsupport.cpp \
    imagejobcontext.cpp \
    searchpaths.cpp \
    recentitem.cpp \
    recentitems.cpp \
    runguard.cpp \
    templatemanifest.cpp \
    versioninfo.cpp \
    util.cpp \
    analytics/analytics.cpp \
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
    dialog/templatebutton.cpp \
    dialog/templategrid.cpp \
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
    script/scriptutil.cpp

HEADERS += chaiscript/chaiscript.hpp \
    cereal/archives/adapters.hpp \
    cereal/archives/binary.hpp \
    cereal/archives/json.hpp \
    cereal/archives/portable_binary.hpp \
    cereal/archives/xml.hpp \
    cereal/details/helpers.hpp \
    cereal/details/polymorphic_impl.hpp \
    cereal/details/polymorphic_impl_fwd.hpp \
    cereal/details/static_object.hpp \
    cereal/details/traits.hpp \
    cereal/details/util.hpp \
    cereal/external/rapidjson/error/en.h \
    cereal/external/rapidjson/error/error.h \
    cereal/external/rapidjson/internal/biginteger.h \
    cereal/external/rapidjson/internal/diyfp.h \
    cereal/external/rapidjson/internal/dtoa.h \
    cereal/external/rapidjson/internal/ieee754.h \
    cereal/external/rapidjson/internal/itoa.h \
    cereal/external/rapidjson/internal/meta.h \
    cereal/external/rapidjson/internal/pow10.h \
    cereal/external/rapidjson/internal/regex.h \
    cereal/external/rapidjson/internal/stack.h \
    cereal/external/rapidjson/internal/strfunc.h \
    cereal/external/rapidjson/internal/strtod.h \
    cereal/external/rapidjson/internal/swap.h \
    cereal/external/rapidjson/msinttypes/inttypes.h \
    cereal/external/rapidjson/msinttypes/stdint.h \
    cereal/external/rapidjson/allocators.h \
    cereal/external/rapidjson/document.h \
    cereal/external/rapidjson/encodedstream.h \
    cereal/external/rapidjson/encodings.h \
    cereal/external/rapidjson/filereadstream.h \
    cereal/external/rapidjson/filewritestream.h \
    cereal/external/rapidjson/fwd.h \
    cereal/external/rapidjson/istreamwrapper.h \
    cereal/external/rapidjson/memorybuffer.h \
    cereal/external/rapidjson/memorystream.h \
    cereal/external/rapidjson/ostreamwrapper.h \
    cereal/external/rapidjson/pointer.h \
    cereal/external/rapidjson/prettywriter.h \
    cereal/external/rapidjson/rapidjson.h \
    cereal/external/rapidjson/reader.h \
    cereal/external/rapidjson/schema.h \
    cereal/external/rapidjson/stream.h \
    cereal/external/rapidjson/stringbuffer.h \
    cereal/external/rapidjson/writer.h \
    cereal/external/rapidxml/rapidxml.hpp \
    cereal/external/rapidxml/rapidxml_iterators.hpp \
    cereal/external/rapidxml/rapidxml_print.hpp \
    cereal/external/rapidxml/rapidxml_utils.hpp \
    cereal/external/base64.hpp \
    cereal/types/concepts/pair_associative_container.hpp \
    cereal/types/array.hpp \
    cereal/types/base_class.hpp \
    cereal/types/bitset.hpp \
    cereal/types/boost_variant.hpp \
    cereal/types/chrono.hpp \
    cereal/types/common.hpp \
    cereal/types/complex.hpp \
    cereal/types/deque.hpp \
    cereal/types/forward_list.hpp \
    cereal/types/functional.hpp \
    cereal/types/list.hpp \
    cereal/types/map.hpp \
    cereal/types/memory.hpp \
    cereal/types/polymorphic.hpp \
    cereal/types/queue.hpp \
    cereal/types/set.hpp \
    cereal/types/stack.hpp \
    cereal/types/string.hpp \
    cereal/types/tuple.hpp \
    cereal/types/unordered_map.hpp \
    cereal/types/unordered_set.hpp \
    cereal/types/utility.hpp \
    cereal/types/valarray.hpp \
    cereal/types/vector.hpp \
    cereal/access.hpp \
    cereal/cereal.hpp \
    cereal/macros.hpp \
    commandlineparser.h \
    constants.h \
    formatsupport.h \
    imagejobcontext.h \
    recentitem.h \
    recentitems.h \
    runguard.h \
    searchpaths.h \
    templatemanifest.h \
    util.h \
    versioninfo.h \
    winversioninfo.h \
    analytics/analytics.h \
    common/sharedapp.h \
    common/uiactions.h \
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
    dialog/runscriptdialog.h \
    dialog/templatebutton.h \
    dialog/templategrid.h \
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
    serialization/templatemetadata.h

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
