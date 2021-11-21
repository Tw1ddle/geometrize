#include "imagetaskgraphicsview.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QTabletEvent>
#include <QTouchEvent>
#include <QWheelEvent>

#include "customtabletevent.h"
#include "scene/imagetaskscene.h"

namespace
{

geometrize::scene::TabletEventData makeCustomTabletEventData(const QTabletEvent& event, const QGraphicsView& view)
{
    const geometrize::scene::TabletEventType eventType = [&event]() {
        switch(event.type()) {
        case QEvent::Type::TabletMove:
            return geometrize::scene::TabletEventType::Move;
        case QEvent::Type::TabletPress:
            return geometrize::scene::TabletEventType::Press;
        case QEvent::Type::TabletRelease:
            return geometrize::scene::TabletEventType::Release;
        case QEvent::Type::TabletEnterProximity:
            return geometrize::scene::TabletEventType::EnterProximity;
        case QEvent::Type::TabletLeaveProximity:
            return geometrize::scene::TabletEventType::LeaveProximity;
        default:
            break;
        }
        return geometrize::scene::TabletEventType::Unknown;
    }();

    #if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    const geometrize::scene::TabletEventPointerType pointerType = [&event]() {
        switch(event.pointerType()) {
        case QPointingDevice::PointerType::Pen:
            return geometrize::scene::TabletEventPointerType::Pen;
        case QPointingDevice::PointerType::Cursor:
            return geometrize::scene::TabletEventPointerType::Cursor;
        case QPointingDevice::PointerType::Eraser:
            return geometrize::scene::TabletEventPointerType::Eraser;
        default:
            break;
        }
        return geometrize::scene::TabletEventPointerType::UnknownPointer;
    }();
    #else
    const geometrize::scene::TabletEventPointerType pointerType = [&event]() {
        switch(event.pointerType()) {
        case QTabletEvent::PointerType::Pen:
            return geometrize::scene::TabletEventPointerType::Pen;
        case QTabletEvent::PointerType::Cursor:
            return geometrize::scene::TabletEventPointerType::Cursor;
        case QTabletEvent::PointerType::Eraser:
            return geometrize::scene::TabletEventPointerType::Eraser;
        default:
            break;
        }
        return geometrize::scene::TabletEventPointerType::UnknownPointer;
    }();
    #endif

    geometrize::scene::TabletEventData data;
    data.eventType = eventType;
    data.pointerType = pointerType;
    #if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    data.xViewPos = static_cast<float>(event.position().x());
    data.yViewPos = static_cast<float>(event.position().y());
    data.xScenePos = static_cast<float>(view.mapToScene(event.position().toPoint()).x());
    data.yScenePos = static_cast<float>(view.mapToScene(event.position().toPoint()).y());
    #else
    data.xViewPos = static_cast<float>(event.pos().x());
    data.yViewPos = static_cast<float>(event.pos().y());
    data.xScenePos = static_cast<float>(view.mapToScene(event.pos()).x());
    data.yScenePos = static_cast<float>(view.mapToScene(event.pos()).y());
    #endif
    data.pressure = static_cast<float>(event.pressure());
    data.tangentialPressure = static_cast<float>(event.tangentialPressure());
    data.xTilt = static_cast<float>(event.xTilt());
    data.yTilt = static_cast<float>(event.yTilt());

    return data;
}

}

namespace geometrize
{

namespace scene
{

class ImageTaskGraphicsView::ImageTaskGraphicsViewImpl
{
public:
    ImageTaskGraphicsViewImpl(ImageTaskGraphicsView* pQ) : q{pQ}, m_totalScaleFactor(1.0f)
    {
    }
    ImageTaskGraphicsViewImpl operator=(const ImageTaskGraphicsViewImpl&) = delete;
    ImageTaskGraphicsViewImpl(const ImageTaskGraphicsViewImpl&) = delete;
    ~ImageTaskGraphicsViewImpl() = default;

    ImageTaskGraphicsView* q;

    float m_totalScaleFactor;
};

ImageTaskGraphicsView::ImageTaskGraphicsView(QWidget* parent) : QGraphicsView(parent), d{std::make_unique<ImageTaskGraphicsViewImpl>(this)}
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    setInteractive(true);
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    populateUi();
}

ImageTaskGraphicsView::~ImageTaskGraphicsView()
{
}

bool ImageTaskGraphicsView::viewportEvent(QEvent* event)
{
    switch(event->type()) {
    case QEvent::TabletMove:
    case QEvent::TabletPress:
    case QEvent::TabletRelease:
    case QEvent::TabletEnterProximity:
    case QEvent::TabletLeaveProximity:
    {
        // Forward an event containing relevant tablet event data to the graphics scene
        const auto customEventData = makeCustomTabletEventData(static_cast<QTabletEvent&>(*event), *this);
        geometrize::scene::CustomTabletEvent customEvent(customEventData, this);
        QCoreApplication::sendEvent(dynamic_cast<geometrize::scene::ImageTaskScene*>(scene()), &customEvent);
        event->accept();
        return true;
    }
    // TODO implement pinch-to-zoom
    /*
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2) {
            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal currentScaleFactor =
                    QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
            if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                // if one of the fingers is released, remember the current scale
                // factor so that adding another finger later will continue zooming
                // by adding new scale factor to the existing remembered value.
                d->m_totalScaleFactor *= currentScaleFactor;
                currentScaleFactor = 1;
            }
            setTransform(QTransform().scale(d->m_totalScaleFactor * currentScaleFactor,
                                            d->m_totalScaleFactor * currentScaleFactor));
        }
        event->accept();
        return true;
    }
    */
    default:
        break;
    }
    return QGraphicsView::viewportEvent(event);
}

void ImageTaskGraphicsView::wheelEvent(QWheelEvent* e)
{
    if(e->modifiers() & Qt::ControlModifier) {
        // Pass down to scene/children in scene if control is pressed
        QGraphicsView::wheelEvent(e);
        return;
    }

    // Handle zooming
    if (e->angleDelta().x() == 0) {
        #if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        const QPointF pos{e->position()};
        const QPointF posf{mapToScene(pos.toPoint())};
        const QPointF newPos{mapToScene(pos.toPoint())};
        #else
        const QPoint pos{e->pos()};
        const QPointF posf{mapToScene(pos)};
        const QPointF newPos{mapToScene(pos)};
        #endif

        const double angle{static_cast<double>(e->angleDelta().y())};
        double by{0.0};
        if(angle > 0) {
            by = 1 + (angle / 360 * 0.1);
        } else if (angle < 0) {
            by = 1 - (-angle / 360 * 0.1);
        } else {
            by = 1;
        }

        scale(by, by);

        const int w{viewport()->width()};
        const int h{viewport()->height()};
        const double wf{mapToScene(QPoint(w - 1, 0)).x() - mapToScene(QPoint(0,0)).x()};
        const double hf{mapToScene(QPoint(0, h - 1)).y() - mapToScene(QPoint(0,0)).y()};
        const double lf{posf.x() - pos.x() * wf / w};
        const double tf{posf.y() - pos.y() * hf / h};

        ensureVisible(lf, tf, wf, hf, 0, 0);
        ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf, QSizeF(wf, hf)), 0, 0);

        e->accept();
    }
}

void ImageTaskGraphicsView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
}

void ImageTaskGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void ImageTaskGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void ImageTaskGraphicsView::keyPressEvent(QKeyEvent* event)
{
    QGraphicsView::keyPressEvent(event);
}

void ImageTaskGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    QGraphicsView::keyReleaseEvent(event);
}

void ImageTaskGraphicsView::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        populateUi();
    }
    QGraphicsView::changeEvent(event);
}

void ImageTaskGraphicsView::populateUi()
{

}

}

}
