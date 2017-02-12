#include "collapsiblesection.h"

#include <QFrame>
#include <QGridLayout>
#include <QLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QToolButton>

namespace geometrize
{

namespace dialog
{

class CollapsibleSection::CollapsibleSectionImpl
{
public:
    CollapsibleSectionImpl(CollapsibleSection* pQ, const QString& title, const int animationDuration) : q{pQ}, animationDuration{animationDuration}
    {
        toggleButton = new QToolButton(q);
        headerLine = new QFrame(q);
        toggleAnimation = new QParallelAnimationGroup(q);
        contentArea = new QScrollArea(q);
        mainLayout = new QGridLayout(q);

        toggleButton->setStyleSheet("QToolButton {border: none;}");
        toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toggleButton->setArrowType(Qt::ArrowType::RightArrow);
        toggleButton->setText(title);
        toggleButton->setCheckable(true);
        toggleButton->setChecked(false);

        headerLine->setFrameShape(QFrame::HLine);
        headerLine->setFrameShadow(QFrame::Sunken);
        headerLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // start out collapsed
        contentArea->setMaximumHeight(0);
        contentArea->setMinimumHeight(0);

        // let the entire widget grow and shrink with its content
        toggleAnimation->addAnimation(new QPropertyAnimation(q, "minimumHeight"));
        toggleAnimation->addAnimation(new QPropertyAnimation(q, "maximumHeight"));
        toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

        mainLayout->setVerticalSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        int row = 0;
        mainLayout->addWidget(toggleButton, row, 0, 1, 1, Qt::AlignLeft);
        mainLayout->addWidget(headerLine, row++, 2, 1, 1);
        mainLayout->addWidget(contentArea, row, 0, 1, 3);
        q->setLayout(mainLayout);

        q->QObject::connect(toggleButton, &QToolButton::clicked, [this](const bool checked)
        {
            toggleButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
            toggleAnimation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
            toggleAnimation->start();
        });
    }
    ~CollapsibleSectionImpl() = default;
    CollapsibleSectionImpl operator=(const CollapsibleSectionImpl&) = delete;
    CollapsibleSectionImpl(const CollapsibleSectionImpl&) = delete;

    void setContentLayout(QLayout& contentLayout)
    {
        delete contentArea->layout();
        contentArea->setLayout(&contentLayout);
        const auto collapsedHeight = q->sizeHint().height() - contentArea->maximumHeight();
        auto contentHeight = contentLayout.sizeHint().height();

        for (int i = 0; i < toggleAnimation->animationCount() - 1; ++i)
        {
            QPropertyAnimation* SectionAnimation = static_cast<QPropertyAnimation *>(toggleAnimation->animationAt(i));
            SectionAnimation->setDuration(animationDuration);
            SectionAnimation->setStartValue(collapsedHeight);
            SectionAnimation->setEndValue(collapsedHeight + contentHeight);
        }

        QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation *>(toggleAnimation->animationAt(toggleAnimation->animationCount() - 1));
        contentAnimation->setDuration(animationDuration);
        contentAnimation->setStartValue(0);
        contentAnimation->setEndValue(contentHeight);
    }

private:
    CollapsibleSection* q;

    QGridLayout* mainLayout;
    QToolButton* toggleButton;
    QFrame* headerLine;
    QParallelAnimationGroup* toggleAnimation;
    QScrollArea* contentArea;
    int animationDuration;
};

CollapsibleSection::CollapsibleSection(QWidget* parent) :
    QGroupBox{parent},
    d{std::make_unique<CollapsibleSection::CollapsibleSectionImpl>(this, "", 100)}
{
}

void CollapsibleSection::setContentLayout(QLayout& contentLayout)
{
    d->setContentLayout(contentLayout);
}

}

}
