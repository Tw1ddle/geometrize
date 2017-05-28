#include "collapsiblepanel.h"

#include <assert.h>

#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QRegularExpression>
#include <QResizeEvent>

namespace geometrize
{

namespace dialog
{

static const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
    unsigned char pixel_data[7 * 10 * 4 + 1];
} expandDownArrow = {
    7, 10, 4,
    "QRY\317445A\0\0\0\0\0\0\0\0\0\0\0\0""445AJJN\317OQW\256OPW\317445#\0\0\0"
    "\0""445#IJP\317HIN\256445#MOT\317LMS\317445#IJP\317GHM\317445#\0\0\0\0""4"
    "45#DEK\317??C\317BBG\317445#\0\0\0\0\0\0\0\0\0\0\0\0""445#>?B\317445#\0\0"
    "\0\0\0\0\0\0LNT\317445A\0\0\0\0\0\0\0\0\0\0\0\0""445ACEI\317JKR\256IJP\317"
    "445#\0\0\0\0""445#DEH\317BCJ\256445#GHO\317EGK\317445#DEH\317BDI\317445#"
    "\0\0\0\0""445#@AE\317??C\317??B\317445#\0\0\0\0\0\0\0\0\0\0\0\0""445#<<?"
    "\317445#\0\0\0\0\0\0\0\0",
};

QIcon createExpandDownIcon()
{
    const QImage img(expandDownArrow.pixel_data, expandDownArrow.width, expandDownArrow.height, QImage::Format_ARGB32);
    return QIcon(QPixmap::fromImage(img));
}

static const QIcon& expandDownIcon()
{
    const static QIcon expandDownIcon(createExpandDownIcon());
    return expandDownIcon;
}

static const struct {
    unsigned int 	 width;
    unsigned int 	 height;
    unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
    unsigned char	 pixel_data[7 * 10 * 4 + 1];
} expandUpArrow = {
    7, 10, 4,
    "\0\0\0\0\0\0\0\0""445#<<?\317445#\0\0\0\0\0\0\0\0\0\0\0\0""445#@AE\317??"
    "C\317??B\317445#\0\0\0\0""445#GHO\317EGK\317445#DEH\317BDI\317445#JKR\256"
    "IJP\317445#\0\0\0\0""445#DEH\317BCJ\256LNT\317445A\0\0\0\0\0\0\0\0\0\0\0"
    "\0""445ACEI\317\0\0\0\0\0\0\0\0""445#>?B\317445#\0\0\0\0\0\0\0\0\0\0\0\0"
    """445#DEK\317??C\317BBG\317445#\0\0\0\0""445#MOT\317LMS\317445#IJP\317GH"
    "M\317445#OQW\256OPW\317445#\0\0\0\0""445#IJP\317HIN\256QRY\317445A\0\0\0"
    "\0\0\0\0\0\0\0\0\0""445AJJN\317",
};

QIcon createExpandUpIcon()
{
    QImage img(expandUpArrow.pixel_data,expandUpArrow.width, expandUpArrow.height, QImage::Format_ARGB32);
    return QIcon(QPixmap::fromImage(img));
}

static const QIcon& expandUpIcon()
{
    const static QIcon expandUpIcon(createExpandUpIcon());
    return expandUpIcon;
}

CollapsiblePanel::CollapsiblePanel(QWidget*)
{
    m_titleFrame = new QFrame(this);
    m_titleFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    m_titleFrame->setAutoFillBackground(true);

    QLinearGradient titleGrad(QPointF(0, 0), QPointF(0, 1));
    titleGrad.setCoordinateMode(QGradient::StretchToDeviceMode);
    titleGrad.setColorAt(0, QColor(255, 255, 255, 20));
    titleGrad.setColorAt(1, QColor(0, 0, 0, 30));

    QPalette titleFramePalette = m_titleFrame->palette();
    titleFramePalette.setBrush(QPalette::Window, titleGrad);
    titleFramePalette.setBrush(QPalette::Foreground, titleFramePalette.dark());
    m_titleFrame->setPalette(titleFramePalette);

    m_titleLabel = new QLabel("", m_titleFrame);
    m_titleLabel->setPalette(QApplication::palette()); // To avoid title foreground color bleeding through

    m_collapseButton = new QPushButton(m_titleFrame);
    m_collapseButton->setFlat(true);
    m_collapseButton->setIcon(expandUpIcon());

    // TODO add code to find the first child (we assume the content frame is the only child of the collapsible panel - this way we can use this with Qt Designer)
    //getContentsPanel()->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    //getContentsPanel()->setAutoFillBackground(true);

    //QPalette contentFramePalette = getContentsPanel()->palette();
    //contentFramePalette.setBrush(QPalette::Window, QColor(255,250,250,85));
    //getContentsPanel()->setPalette(contentFramePalette);

    connect(m_collapseButton, SIGNAL(clicked()),this, SLOT(toggleExpanded()) );
}

void CollapsiblePanel::setTitle(const QString& title)
{
    m_titleLabel->setText(title);
}

void CollapsiblePanel::setExpanded(bool isExpanded)
{
    getContentsPanel()->setVisible(isExpanded);
    isExpanded ? m_collapseButton->setIcon(expandUpIcon()) : m_collapseButton->setIcon(expandDownIcon());
    QWidget::updateGeometry();
}

void CollapsiblePanel::toggleExpanded()
{
    setExpanded(!getContentsPanel()->isVisible());
}

QSize CollapsiblePanel::minimumSizeHint() const
{
    QSize labelSize =  m_titleLabel->sizeHint();
    QSize titleBarHint = labelSize + QSize(4 + labelSize.height() + 8 - 2 + 1, 8);

    if (getContentsPanel()->isVisible())
    {
        QSize titleBarMin(0, labelSize.height() + 8);
        QFrame* panel = getContentsPanel();
        QSize contentsMin(panel->minimumSizeHint());
        QSize total = contentsMin.expandedTo(titleBarMin);
        total.rheight() += titleBarMin.height();
        return total;
    }
    else
    {
        return titleBarHint;
    }
}

void CollapsiblePanel::resizeEvent(QResizeEvent *resizeEv)
{
    QWidget::updateGeometry();

    int width = resizeEv->size().width();
    int height = resizeEv->size().height();
    int labelHeight = m_titleLabel->sizeHint().height();
    int titleHeight = labelHeight + 8;

    int buttonSize = titleHeight - 2;

    m_titleFrame->setGeometry(0, 0, width, titleHeight);
    m_titleLabel->setGeometry( 4, titleHeight - labelHeight - 4, width - buttonSize - 1, labelHeight);
    m_collapseButton->setGeometry(width - buttonSize - 1, 1, buttonSize, buttonSize);
    getContentsPanel()->setGeometry(0, titleHeight - 1, width, height - titleHeight -1 );
}

bool CollapsiblePanel::event(QEvent* event)
{
    if (event->type() == QEvent::LayoutRequest)
    {
        this->QWidget::updateGeometry();
    }
    return QWidget::event(event);
}

QFrame* CollapsiblePanel::getContentsPanel() const
{
    const QList<QFrame*>& children = this->findChildren<QFrame*>(QRegularExpression(".*ContentsFrame$"), Qt::FindDirectChildrenOnly);
    if(children.isEmpty()) {
        assert(0 && "Could not find the panel contents for the collapsible panel");
        return nullptr;
    }

    QObject* child = children[0];
    QFrame* frame = static_cast<QFrame*>(child);
    return frame;
}

}

}
