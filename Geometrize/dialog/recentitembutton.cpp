#include "recentitembutton.h"
#include "ui_recentitembutton.h"

namespace geometrize
{

namespace dialog
{

RecentItemButton::RecentItemButton(const QString& item) :
    ui(new Ui::RecentItemButton),
    m_item{item}
{
    ui->setupUi(this);

    // TODO infer type of item and get image from cache or generate one/grab icon
    // TODO setup appropriate click action based on item type
}

RecentItemButton::~RecentItemButton()
{
    delete ui;
}

}

}
