#include "recentitemwidget.h"
#include "ui_recentitemwidget.h"

namespace geometrize
{

namespace dialog
{

RecentItemWidget::RecentItemWidget(const RecentItem& item) :
    ui(new Ui::RecentItemWidget),
    m_item{item}
{
    ui->setupUi(this);

    ui->itemDisplayName->setText(item.getDisplayName());

    const QString key{item.getKey()};
    ui->itemPath->setText(key);

    // TODO infer item type
    ui->itemTypeIcon->setPixmap(QPixmap(":/icons/folder_image.png"));
}

RecentItemWidget::~RecentItemWidget()
{
    delete ui;
}

}

}
