#include "collapsiblepanel.h"

#include <cassert>

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QRegularExpression>
#include <QScrollArea>
#include <QToolButton>

namespace geometrize
{

namespace dialog
{

class CollapsiblePanel::CollapsiblePanelImpl
{
public:
    CollapsiblePanelImpl(CollapsiblePanel* pQ, const int animationDuration) : q{pQ}, animationDuration{animationDuration}
    {
    }
    ~CollapsiblePanelImpl() = default;
    CollapsiblePanelImpl operator=(const CollapsiblePanelImpl&) = delete;
    CollapsiblePanelImpl(const CollapsiblePanelImpl&) = delete;

    void setup(const QString& title)
    {
        QGridLayout* layout = dynamic_cast<QGridLayout*>(q->layout());
        if(layout == nullptr) {
            assert(0 && "Failed to get collapsible panel layout. The collapsible panel layout needs to be a QGridLayout i.e. the contents need to be laid out in a grid");
            return;
        }
        layout->setContentsMargins(0, 0, 0, 0);

        panelHeaderFrame = new QFrame();
        panelHeaderFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QHBoxLayout* boxLayout = new QHBoxLayout();
        boxLayout->setContentsMargins(0, 0, 0, 0);
        panelHeaderFrame->setLayout(boxLayout);

        toggleButton = new QToolButton();
        toggleButton->setStyleSheet("QToolButton {border: none;}");
        toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toggleButton->setArrowType(Qt::ArrowType::RightArrow);
        toggleButton->setCheckable(true);
        toggleButton->setChecked(false);
        toggleButton->setText(title);

        boxLayout->addWidget(toggleButton);

        layout->addWidget(panelHeaderFrame, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);

        QScrollArea* contentArea = getContentArea();
        if(contentArea == nullptr) {
            assert(0 && "Could not find the collapsible panel contents/scroll area widget i.e. the collapsible panel needs to have a direct ScrollArea child with a name ending in ScrollArea");
            return;
        }
        contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        contentArea->setMaximumHeight(0);
        contentArea->setMinimumHeight(0);
        contentArea->resize(contentArea->size().width(), 0);

        const int collapsedHeight{q->sizeHint().height() - contentArea->maximumHeight()};
        const int contentHeight{contentArea->sizeHint().height()};

        toggleAnimation = new QParallelAnimationGroup(q);
        toggleAnimation->addAnimation(new QPropertyAnimation(q, "minimumHeight"));
        toggleAnimation->addAnimation(new QPropertyAnimation(q, "maximumHeight"));
        toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));
        for (int i = 0; i < toggleAnimation->animationCount() - 1; i++) {
            QPropertyAnimation* sectionAnimation = static_cast<QPropertyAnimation*>(toggleAnimation->animationAt(i));
            sectionAnimation->setDuration(animationDuration);
            sectionAnimation->setStartValue(collapsedHeight);
            sectionAnimation->setEndValue(collapsedHeight + contentHeight);
        }

        QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation*>(toggleAnimation->animationAt(toggleAnimation->animationCount() - 1));
        contentAnimation->setDuration(animationDuration);
        contentAnimation->setStartValue(0);
        contentAnimation->setEndValue(contentHeight);

        q->QObject::connect(toggleButton, &QToolButton::clicked, [this](const bool checked) {
            toggleButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
            toggleAnimation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
            toggleAnimation->start();
        });

        q->updateGeometry();
    }

private:
    QScrollArea* getContentArea() const
    {
        const QList<QScrollArea*>& children = q->findChildren<QScrollArea*>(QRegularExpression(".*ScrollArea$"), Qt::FindDirectChildrenOnly);
        if(children.isEmpty() || children.size() != 1) {
            return nullptr;
        }
        return children[0];
    }

    CollapsiblePanel* q;
    QFrame* panelHeaderFrame;
    QToolButton* toggleButton;
    QParallelAnimationGroup* toggleAnimation;
    const int animationDuration;
};

CollapsiblePanel::CollapsiblePanel(QWidget* parent) :
    QFrame{parent},
    d{std::make_unique<CollapsiblePanel::CollapsiblePanelImpl>(this, 300)}
{
}

CollapsiblePanel::~CollapsiblePanel()
{
}

void CollapsiblePanel::setup(const QString& title)
{
    d->setup(title);
}

}

}
