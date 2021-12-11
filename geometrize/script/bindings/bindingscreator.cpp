#include "script/bindings/bindingscreator.h"

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

#include <QImage>
#include <QObject>
#include <QWidget>
#include <Qt>

#include "chaiscript/chaiscript.hpp"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/exporter/bitmapdataexporter.h"
#include "geometrize/exporter/bitmapexporter.h"
#include "geometrize/exporter/shapearrayexporter.h"
#include "geometrize/exporter/shapejsonexporter.h"
#include "geometrize/exporter/svgexporter.h"
#include "geometrize/rasterizer/rasterizer.h"
#include "geometrize/runner/imagerunner.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/shape/circle.h"
#include "geometrize/shape/ellipse.h"
#include "geometrize/shape/line.h"
#include "geometrize/shape/polyline.h"
#include "geometrize/shape/quadraticbezier.h"
#include "geometrize/shape/rectangle.h"
#include "geometrize/shape/rotatedellipse.h"
#include "geometrize/shape/rotatedrectangle.h"
#include "geometrize/shape/shapefactory.h"
#include "geometrize/shape/shapetypes.h"
#include "geometrize/shape/triangle.h"
#include "geometrize/commonutil.h"
#include "geometrize/model.h"
#include "geometrize/shaperesult.h"

#include "common/uiactions.h"
#include "dialog/launchwindow.h"
#include "dialog/imagetaskwindow.h"
#include "exporter/imageexporter.h"
#include "image/imageloader.h"
#include "scene/customtabletevent.h"
#include "script/bindings/bindingshelpers.h"
#include "script/bindings/bindingswrapper.h"
#include "script/bindings/chaiscriptmathextras.h"
#include "script/scriptutil.h"
#include "task/imagetask.h"
#include "task/synchronousimagetask.h"

namespace
{

std::shared_ptr<chaiscript::Module> createModule()
{
    auto module = std::make_shared<chaiscript::Module>();

    chaiscript::bootstrap::standard_library::vector_type<std::vector<int>>("IntVector", *module);
    chaiscript::bootstrap::standard_library::vector_type<std::vector<float>>("FloatVector", *module);
    chaiscript::bootstrap::standard_library::vector_type<std::vector<std::string>>("StringVector", *module);
    chaiscript::bootstrap::standard_library::pair_type<std::pair<int, int>>("IntPair", *module);
    chaiscript::bootstrap::standard_library::pair_type<std::pair<float, float>>("FloatPair", *module);
    chaiscript::bootstrap::standard_library::vector_type<std::vector<std::pair<int, int>>>("IntPairVector", *module);
    chaiscript::bootstrap::standard_library::vector_type<std::vector<std::pair<float, float>>>("FloatPairVector", *module);

    return module;
}

}

namespace geometrize
{

namespace script
{

namespace bindings
{

std::shared_ptr<chaiscript::Module> createDefaultBindings()
{
    auto module = createModule();

    ADD_FREE_FUN(sleep);

    ADD_FREE_FUN(debugBreak);
    ADD_FREE_FUN(messageBox);
    ADD_FREE_FUN(printToAllScriptConsoleWidgets);
    ADD_FREE_FUN(printToConsole);

    ADD_FREE_FUN(processApplicationEvents);
    ADD_FREE_FUN(directoryContainsFile);
    ADD_FREE_FUN(createDirectory);
    ADD_FREE_FUN(fileExists);
    ADD_FREE_FUN(directoryExists);
    ADD_FREE_FUN(directoriesExist);
    ADD_FREE_FUN(readFileAsString);
    ADD_FREE_FUN(getFilePathsForDirectory);

    ADD_FREE_FUN(getScriptSearchPaths);
    ADD_FREE_FUN(getTemplateSearchPaths);

    ADD_FREE_FUN(getApplicationDirectoryPath);

    ADD_FREE_FUN(getFilesWithExtension);
    ADD_FREE_FUN(getFilesWithExtensions);
    ADD_FREE_FUN(getFirstFileWithExtension);
    ADD_FREE_FUN(getFirstFileWithExtensions);
    ADD_FREE_FUN(getScriptsForPath);
    ADD_FREE_FUN(getScriptsForPaths);
    ADD_FREE_FUN(getFileNameForFilePath);
    ADD_FREE_FUN(getDirectoryForFilePath);

    ADD_FREE_FUN(getSupportedImageFileExtensions);

    ADD_FREE_FUN(openTask);

    ADD_FREE_FUN(openInDefaultApplication);
    ADD_FREE_FUN(revealInDefaultApplication);

    ADD_FREE_FUN(clearGlobalClipboard);
    ADD_FREE_FUN(getGlobalClipboardText);
    ADD_FREE_FUN(setGlobalClipboardText);

    ADD_FREE_FUN(stringBeginsWith);
    ADD_FREE_FUN(stringEndsWith);

    ADD_FREE_FUN(getApplicationDirectoryLocation);
    ADD_FREE_FUN(getAppDataLocation);
    ADD_FREE_FUN(getHomeDirectoryLocation);
    ADD_FREE_FUN(getDesktopDirectoryLocation);

    ADD_FREE_FUN(writeStringToFile);

    ADD_FREE_FUN(percentEncode);

    ADD_FREE_FUN_TEMPLATE(randomInRange, float COMMA float COMMA float);
    ADD_FREE_FUN_TEMPLATE(clamp, float COMMA float COMMA float);

    // Some specialized helper templates if in doubt
    ADD_FREE_FUN(randomIntInRange);
    ADD_FREE_FUN(randomFloatInRange);

    ADD_FREE_FUN(clampInt);
    ADD_FREE_FUN(clampFloat);

    ADD_FREE_FUN_TEMPLATE(randomInVector, std::pair<int COMMA int>);
    ADD_FREE_FUN_TEMPLATE(randomInVector, std::pair<float COMMA float>);

    ADD_FREE_FUN_TEMPLATE(vectorContains, std::pair<int COMMA int>);
    ADD_FREE_FUN_TEMPLATE(vectorContains, std::pair<float COMMA float>);

    ADD_FREE_FUN(split);

    ADD_FREE_FUN(getOperatingSystemProductType);

    ADD_FREE_FUN(setTranslatorsForLocale);
    ADD_FREE_FUN(getSupportedLocaleCodes);
    ADD_FREE_FUN(setLocaleAndUserInterfaceLanguage);

    ADD_FREE_FUN(getAllNamedWidgets);
    ADD_FREE_FUN(getAllNamedGeometrizeWidgets);
    ADD_FREE_FUN(getWidgetByName);

    ADD_TYPE(Command);
    ADD_MEMBER(Command, getCommand);
    ADD_MEMBER(Command, setStringArg);

    ADD_TYPE(CommandHandler);
    ADD_MEMBER(CommandHandler, handleCommand);
    ADD_MEMBER(CommandHandler, getCommandHandlerName);
    ADD_MEMBER(CommandHandler, setCommandHandlerName);

    ADD_FREE_FUN(getAllNamedCommandHandlers);
    ADD_FREE_FUN(getCommandHandlerByName);

    ADD_FREE_FUN(sendCommandString);
    ADD_FREE_FUN(sendCommandObject);

    ADD_FREE_FUN(broadcastCommandString);
    ADD_FREE_FUN(broadcastCommandObject);

    ADD_FREE_FUN(getFormattedTimestamp);
    ADD_FREE_FUN(getFilenameTimestamp);

    ADD_FREE_FUN(getUuidString);

    return module;
}

std::shared_ptr<chaiscript::Module> createQWidgetBindings()
{
    auto module = createModule();

    ADD_TYPE(QWidget);

    ADD_BASE_CLASS(QObject, QWidget);

    ADD_MEMBER(QWidget, show);
    ADD_MEMBER(QWidget, hide);
    ADD_MEMBER(QWidget, updateGeometry);

    ADD_FREE_FUN(setWidgetSize);
    ADD_FREE_FUN(setWidgetPosition);

    ADD_FREE_FUN(arrangeWidgetsInGrid);
    ADD_FREE_FUN(arrangeWidgetsInGridOnMonitor);
    ADD_FREE_FUN(arrangeWidgetsInGridOnPrimaryMonitor);
    ADD_FREE_FUN(fitWidgetsInGridOnMonitor);
    ADD_FREE_FUN(fitWidgetsInGridOnPrimaryMonitor);

    module->add(chaiscript::fun(static_cast<void(QWidget::*)(int, int)>(&QWidget::resize)), "resize");
    module->add(chaiscript::fun(static_cast<void(QWidget::*)(int, int)>(&QWidget::move)), "move");

    return module;
}

std::shared_ptr<chaiscript::Module> createLaunchWindowBindings()
{
    using namespace geometrize::dialog;
    using namespace geometrize::common::ui;

    auto module = createModule();

    ADD_TYPE(LaunchWindow);

    ADD_BASE_CLASS(QMainWindow, LaunchWindow);
    ADD_BASE_CLASS(QWidget, LaunchWindow);
    ADD_BASE_CLASS(QObject, LaunchWindow);
    ADD_BASE_CLASS(CommandHandler, LaunchWindow);

    ADD_CONSTRUCTOR(LaunchWindow, LaunchWindow());

    ADD_FREE_FUN(createLaunchWindow);

    chaiscript::bootstrap::standard_library::vector_type<std::vector<LaunchWindow*>>("LaunchWindowVector", *module);
    ADD_FREE_FUN(getLaunchWindows);

    ADD_MEMBER(LaunchWindow, show);
    ADD_MEMBER(LaunchWindow, hide);
    ADD_MEMBER(LaunchWindow, updateGeometry);

    module->add(chaiscript::fun(static_cast<void(LaunchWindow::*)(int, int)>(&LaunchWindow::resize)), "resize");
    module->add(chaiscript::fun(static_cast<void(LaunchWindow::*)(int, int)>(&LaunchWindow::move)), "move");

    return module;
}

std::shared_ptr<chaiscript::Module> createImageTaskWindowBindings()
{
    using namespace geometrize::dialog;
    using namespace geometrize::common::ui;

    auto module = createModule();

    ADD_TYPE(ImageTaskWindow);

    ADD_BASE_CLASS(QMainWindow, ImageTaskWindow);
    ADD_BASE_CLASS(QWidget, ImageTaskWindow);
    ADD_BASE_CLASS(QObject, ImageTaskWindow);
    ADD_BASE_CLASS(CommandHandler, ImageTaskWindow);

    ADD_CONSTRUCTOR(ImageTaskWindow, ImageTaskWindow());

    ADD_FREE_FUN(createImageTaskWindow);
    ADD_FREE_FUN(arrangeImageTaskWidgetsInGrid);
    ADD_FREE_FUN(arrangeImageTaskWidgetsInGridOnMonitor);
    ADD_FREE_FUN(arrangeImageTaskWidgetsInGridOnPrimaryMonitor);
    ADD_FREE_FUN(fitImageTaskWidgetsInGridOnMonitor);
    ADD_FREE_FUN(fitImageTaskWidgetsInGridOnPrimaryMonitor);

    chaiscript::bootstrap::standard_library::vector_type<std::vector<ImageTaskWindow*>>("ImageTaskWindowVector", *module);
    ADD_FREE_FUN(getImageTaskWindows);

    ADD_MEMBER(ImageTaskWindow, show);
    ADD_MEMBER(ImageTaskWindow, hide);
    ADD_MEMBER(ImageTaskWindow, getImageTask);
    ADD_MEMBER(ImageTaskWindow, setImageTask);

    module->add(chaiscript::fun(static_cast<void(ImageTaskWindow::*)(int, int)>(&ImageTaskWindow::resize)), "resize");
    module->add(chaiscript::fun(static_cast<void(ImageTaskWindow::*)(int, int)>(&ImageTaskWindow::move)), "move");

    return module;
}

std::shared_ptr<chaiscript::Module> createImageBindings()
{
    using namespace geometrize::image;

    auto module = createModule();

    ADD_TYPE(QImage);

    ADD_MEMBER(QImage, sizeInBytes);
    ADD_MEMBER(QImage, colorCount);
    ADD_MEMBER(QImage, depth);
    ADD_MEMBER(QImage, isNull);
    ADD_MEMBER(QImage, width);
    ADD_MEMBER(QImage, height);

    chaiscript::utility::add_class<Qt::TransformationMode>(*module,
      "TransformationMode",
    {
      { Qt::FastTransformation, "QtFastTransformation" },
      { Qt::SmoothTransformation, "QtSmoothTransformation" }
    });
    ADD_MEMBER(QImage, scaledToWidth);
    ADD_MEMBER(QImage, scaledToHeight);

    ADD_FREE_FUN(loadImage);
    ADD_FREE_FUN(convertImageToRgba8888);
    ADD_FREE_FUN(createBitmap);
    ADD_FREE_FUN(convertImageToBitmapWithDownscaling);

    return module;
}

std::shared_ptr<chaiscript::Module> createSynchronousImageTaskBindings()
{
    using namespace geometrize::task;
    using namespace geometrize::preferences;

    auto module = createModule();

    // Note we use synchronous-stepping image tasks for simplicity in some scripts e.g. the Twitter bot
    ADD_TYPE(SynchronousImageTask);

    ADD_CONSTRUCTOR(SynchronousImageTask, SynchronousImageTask(Bitmap&));

    ADD_MEMBER(SynchronousImageTask, stepModel);
    ADD_MEMBER(SynchronousImageTask, drawBackgroundRectangle);
    ADD_MEMBER(SynchronousImageTask, getTarget);
    ADD_MEMBER(SynchronousImageTask, getCurrent);
    ADD_MEMBER(SynchronousImageTask, getPreferences);
    ADD_MEMBER(SynchronousImageTask, setPreferences);
    ADD_MEMBER(SynchronousImageTask, getShapes);

    return module;
}

std::shared_ptr<chaiscript::Module> createImageTaskBindings()
{
    using namespace geometrize::task;
    using namespace geometrize::preferences;

    auto module = createModule();

    ADD_TYPE(ImageTask);

    ADD_CONSTRUCTOR(ImageTask, ImageTask(Bitmap&));

    ADD_MEMBER(ImageTask, stepModel);
    ADD_MEMBER(ImageTask, drawBackgroundRectangle);
    ADD_MEMBER(ImageTask, getTarget);
    ADD_MEMBER(ImageTask, getCurrent);
    ADD_MEMBER(ImageTask, getTargetMutable);
    ADD_MEMBER(ImageTask, getCurrentMutable);
    ADD_MEMBER(ImageTask, getPreferences);
    ADD_MEMBER(ImageTask, setPreferences);
    ADD_MEMBER(ImageTask, getDisplayName);
    ADD_MEMBER(ImageTask, getWidth);
    ADD_MEMBER(ImageTask, getHeight);
    ADD_MEMBER(ImageTask, getTaskId);
    ADD_MEMBER(ImageTask, isStepping);

    ADD_FREE_FUN(createImageTask);

    ADD_TYPE(ImageTaskPreferences);

    ADD_CONSTRUCTOR(ImageTaskPreferences, ImageTaskPreferences());
    ADD_CONSTRUCTOR(ImageTaskPreferences, ImageTaskPreferences(const std::string&));
    ADD_CONSTRUCTOR(ImageTaskPreferences, ImageTaskPreferences(const ImageTaskPreferences&));

    ADD_MEMBER(ImageTaskPreferences, enableShapeTypes);
    ADD_MEMBER(ImageTaskPreferences, disableShapeTypes);
    ADD_MEMBER(ImageTaskPreferences, setShapeTypes);
    ADD_MEMBER(ImageTaskPreferences, setShapeAlpha);
    ADD_MEMBER(ImageTaskPreferences, setCandidateShapeCount);
    ADD_MEMBER(ImageTaskPreferences, setMaxShapeMutations);
    ADD_MEMBER(ImageTaskPreferences, setSeed);
    ADD_MEMBER(ImageTaskPreferences, setMaxThreads);

    module->add(chaiscript::fun(
                    static_cast<void(ImageTaskPreferences::*)(double, double, double, double)>
                    (&ImageTaskPreferences::setShapeBounds)), "setShapeBounds");

    ADD_MEMBER(ImageTaskPreferences, setShapeBoundsXMinPercent);
    ADD_MEMBER(ImageTaskPreferences, setShapeBoundsYMinPercent);
    ADD_MEMBER(ImageTaskPreferences, setShapeBoundsXMaxPercent);
    ADD_MEMBER(ImageTaskPreferences, setShapeBoundsYMaxPercent);
    ADD_MEMBER(ImageTaskPreferences, setShapeBoundsEnabled);

    ADD_MEMBER(ImageTaskPreferences, isScriptModeEnabled);
    ADD_MEMBER(ImageTaskPreferences, setScriptModeEnabled);
    ADD_MEMBER(ImageTaskPreferences, setScript);

    return module;
}

std::shared_ptr<chaiscript::Module> createImageExportBindings()
{
    using namespace geometrize::exporter;

    auto module = createModule();

    ADD_FREE_FUN(exportBitmap);
    ADD_FREE_FUN(exportImage);
    ADD_FREE_FUN(exportRasterizedSvg);

    return module;
}

std::shared_ptr<chaiscript::Module> createAnimatedGifExportBindings()
{
    using namespace geometrize::script::bindings;

    auto module = createModule();

    ADD_FREE_FUN(exportGIF);

    return module;
}

std::shared_ptr<chaiscript::Module> createGeometrizeLibraryBindings()
{
    auto module = createModule();

    ADD_TYPE(Bitmap);
    ADD_CONSTRUCTOR(Bitmap, Bitmap(std::uint32_t COMMA std::uint32_t COMMA geometrize::rgba));
    ADD_CONSTRUCTOR(Bitmap, Bitmap(std::uint32_t COMMA std::uint32_t COMMA const std::vector<std::uint8_t>& data));
    ADD_CONSTRUCTOR(Bitmap, Bitmap(const geometrize::Bitmap&));
    ADD_MEMBER(Bitmap, getWidth);
    ADD_MEMBER(Bitmap, getHeight);
    ADD_MEMBER(Bitmap, copyData);
    ADD_MEMBER(Bitmap, getDataRef);
    ADD_MEMBER(Bitmap, getPixel);
    ADD_MEMBER(Bitmap, setPixel);
    ADD_MEMBER(Bitmap, fill);

    ADD_TYPE(rgba);
    ADD_MEMBER(rgba, r);
    ADD_MEMBER(rgba, g);
    ADD_MEMBER(rgba, b);
    ADD_MEMBER(rgba, a);

    using namespace geometrize::exporter;

    ADD_FREE_FUN(exportBitmapData);
    ADD_FREE_FUN(exportBitmap);
    ADD_FREE_FUN(exportShapeArray);
    ADD_FREE_FUN(exportShapeJson);
    ADD_FREE_FUN(exportSVG);

    ADD_TYPE(ImageRunner);
    ADD_CONSTRUCTOR(ImageRunner, ImageRunner(const geometrize::Bitmap&));
    ADD_CONSTRUCTOR(ImageRunner, ImageRunner(const geometrize::Bitmap& COMMA const geometrize::Bitmap&));
    ADD_MEMBER(ImageRunner, step);
    //ADD_MEMBER(ImageRunner, getCurrent);
    //ADD_MEMBER(ImageRunner, getTarget);
    ADD_MEMBER(ImageRunner, getModel);

    ADD_TYPE(Shape);
    ADD_MEMBER(Shape, getType);
    ADD_MEMBER(Shape, clone);

    ADD_BASE_CLASS(Shape, Circle);
    ADD_TYPE(Circle);
    ADD_CONSTRUCTOR(Circle, Circle(float COMMA float COMMA float));
    ADD_MEMBER(Circle, m_x);
    ADD_MEMBER(Circle, m_y);
    ADD_MEMBER(Circle, m_r);

    ADD_BASE_CLASS(Shape, Ellipse);
    ADD_TYPE(Ellipse);
    ADD_CONSTRUCTOR(Ellipse, Ellipse(float COMMA float COMMA float COMMA float));
    ADD_MEMBER(Ellipse, m_x);
    ADD_MEMBER(Ellipse, m_y);
    ADD_MEMBER(Ellipse, m_rx);
    ADD_MEMBER(Ellipse, m_ry);

    ADD_BASE_CLASS(Shape, Line);
    ADD_TYPE(Line);
    ADD_CONSTRUCTOR(Line, Line(float COMMA float COMMA float COMMA float));
    ADD_MEMBER(Line, m_x1);
    ADD_MEMBER(Line, m_y1);
    ADD_MEMBER(Line, m_x2);
    ADD_MEMBER(Line, m_y2);

    ADD_BASE_CLASS(Shape, Polyline);
    ADD_TYPE(Polyline);
    ADD_CONSTRUCTOR(Polyline, Polyline(const std::vector<std::pair<float, float>>));

    // Make the polyline points vector accessible from scripts
    chaiscript::bootstrap::standard_library::pair_type<std::pair<float, float>>("FloatPair", *module);
    chaiscript::bootstrap::standard_library::vector_type<std::vector<std::pair<float, float>>>("IntPairVector", *module);
    module->add(chaiscript::vector_conversion<std::vector<std::pair<float, float>>>());
    ADD_MEMBER(Polyline, m_points);

    ADD_BASE_CLASS(Shape, QuadraticBezier);
    ADD_TYPE(QuadraticBezier);
    ADD_CONSTRUCTOR(QuadraticBezier, QuadraticBezier(float COMMA float COMMA float COMMA float COMMA float COMMA float));
    ADD_MEMBER(QuadraticBezier, m_cx);
    ADD_MEMBER(QuadraticBezier, m_cy);
    ADD_MEMBER(QuadraticBezier, m_x1);
    ADD_MEMBER(QuadraticBezier, m_y1);
    ADD_MEMBER(QuadraticBezier, m_x2);
    ADD_MEMBER(QuadraticBezier, m_y2);

    ADD_BASE_CLASS(Shape, Rectangle);
    ADD_TYPE(Rectangle);
    ADD_CONSTRUCTOR(Rectangle, Rectangle(float COMMA float COMMA float COMMA float));
    ADD_MEMBER(Rectangle, m_x1);
    ADD_MEMBER(Rectangle, m_y1);
    ADD_MEMBER(Rectangle, m_x2);
    ADD_MEMBER(Rectangle, m_y2);

    ADD_BASE_CLASS(Shape, RotatedEllipse);
    ADD_TYPE(RotatedEllipse);
    ADD_CONSTRUCTOR(RotatedEllipse, RotatedEllipse(float COMMA float COMMA float COMMA float COMMA float));
    ADD_MEMBER(RotatedEllipse, m_x);
    ADD_MEMBER(RotatedEllipse, m_y);
    ADD_MEMBER(RotatedEllipse, m_rx);
    ADD_MEMBER(RotatedEllipse, m_ry);
    ADD_MEMBER(RotatedEllipse, m_angle);

    ADD_BASE_CLASS(Shape, RotatedRectangle);
    ADD_TYPE(RotatedRectangle);
    ADD_CONSTRUCTOR(RotatedRectangle, RotatedRectangle(float COMMA float COMMA float COMMA float COMMA float));
    ADD_MEMBER(RotatedRectangle, m_x1);
    ADD_MEMBER(RotatedRectangle, m_y1);
    ADD_MEMBER(RotatedRectangle, m_x2);
    ADD_MEMBER(RotatedRectangle, m_y2);
    ADD_MEMBER(RotatedRectangle, m_angle);

    ADD_BASE_CLASS(Shape, Triangle);
    ADD_TYPE(Triangle);
    ADD_CONSTRUCTOR(Triangle, Triangle(float COMMA float COMMA float COMMA float COMMA float COMMA float));
    ADD_MEMBER(Triangle, m_x1);
    ADD_MEMBER(Triangle, m_y1);
    ADD_MEMBER(Triangle, m_x2);
    ADD_MEMBER(Triangle, m_y2);
    ADD_MEMBER(Triangle, m_x3);
    ADD_MEMBER(Triangle, m_y3);

    chaiscript::utility::add_class<geometrize::ShapeTypes>(*module,
      "ShapeTypes",
    {
      { RECTANGLE, "RECTANGLE" },
      { ROTATED_RECTANGLE, "ROTATED_RECTANGLE" },
      { TRIANGLE, "TRIANGLE" },
      { ELLIPSE, "ELLIPSE" },
      { ROTATED_ELLIPSE, "ROTATED_ELLIPSE" },
      { CIRCLE, "CIRCLE" },
      { LINE, "LINE" },
      { QUADRATIC_BEZIER, "QUADRATIC_BEZIER" },
      { POLYLINE, "POLYLINE" }
    });

    using namespace geometrize::core;

    ADD_FREE_FUN(defaultEnergyFunction);

    ADD_TYPE(Scanline);
    ADD_MEMBER(Scanline, y);
    ADD_MEMBER(Scanline, x1);
    ADD_MEMBER(Scanline, x2);

    chaiscript::bootstrap::standard_library::vector_type<std::vector<Scanline>>("ScanlineVector", *module);

    using namespace geometrize::commonutil;

    ADD_FREE_FUN(seedRandomGenerator);
    ADD_FREE_FUN(getAverageImageColor);

    ADD_FREE_FUN(shapesOverlap);
    ADD_FREE_FUN(shapeContains);

    ADD_FREE_FUN(shapeToPixels);

    ADD_FREE_FUN(scanlinesContainTransparentPixels);

    ADD_TYPE(Model);
    ADD_CONSTRUCTOR(Model, Model(const geometrize::Bitmap&));
    ADD_CONSTRUCTOR(Model, Model(const geometrize::Bitmap& COMMA const geometrize::Bitmap&));

    ADD_MEMBER(Model, reset);
    ADD_MEMBER(Model, getWidth);
    ADD_MEMBER(Model, getHeight);
    ADD_MEMBER(Model, step);
    //ADD_MEMBER(Model, drawShape);
    //ADD_MEMBER(Model, getCurrent);
    //ADD_MEMBER(Model, getTarget);
    ADD_MEMBER(Model, setSeed);

    return module;
}

std::shared_ptr<chaiscript::Module> createTabletEventBindings()
{
    using namespace geometrize::scene;

    auto module = createModule();

    chaiscript::utility::add_class<TabletEventType>(*module,
      "TabletEventType",
    {
        { TabletEventType::Move, "Move" },
        { TabletEventType::Press, "Press" },
        { TabletEventType::Release, "Release" },
        { TabletEventType::EnterProximity, "EnterProximity" },
        { TabletEventType::LeaveProximity, "LeaveProximity" },
        { TabletEventType::Unknown, "Unknown" }
    });

    chaiscript::utility::add_class<TabletEventPointerType>(*module,
      "TabletEventPointerType",
    {
      { TabletEventPointerType::UnknownPointer, "UnknownPointer" },
      { TabletEventPointerType::Pen, "Pen" },
      { TabletEventPointerType::Cursor, "Cursor" },
      { TabletEventPointerType::Eraser, "Eraser" }
    });

    ADD_TYPE(TabletEventData);
    ADD_MEMBER(TabletEventData, eventType);
    ADD_MEMBER(TabletEventData, pointerType);
    ADD_MEMBER(TabletEventData, xViewPos);
    ADD_MEMBER(TabletEventData, yViewPos);
    ADD_MEMBER(TabletEventData, xScenePos);
    ADD_MEMBER(TabletEventData, yScenePos);
    ADD_MEMBER(TabletEventData, pressure);
    ADD_MEMBER(TabletEventData, tangentialPressure);
    ADD_MEMBER(TabletEventData, rotation);
    ADD_MEMBER(TabletEventData, xTilt);
    ADD_MEMBER(TabletEventData, yTilt);

    ADD_FREE_FUN(customTabletEventDataToString);

    return module;
}

std::shared_ptr<chaiscript::Module> createMathBindings()
{
    return chaiscript::extras::math::bootstrap();
}

std::shared_ptr<chaiscript::Module> createUserInterfacePuppeteerBindings()
{
    auto module = createModule();

    ADD_FREE_FUN(getCursorX);
    ADD_FREE_FUN(getCursorY);
    ADD_FREE_FUN(setCursorPos);

    return module;
}

std::shared_ptr<chaiscript::Module> createFunctionalTestHelperBindings()
{
    auto module = createModule();

    ADD_FREE_FUN(saveWidgetScreenshot);

    return module;
}

}

}

}
