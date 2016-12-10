#include "itembutton.h"
#include "ui_itembutton.h"

namespace geometrize
{

namespace dialog
{

ItemButton::ItemButton(const QString& item) :
    ui(new Ui::ItemButton),
    m_item{item}
{
    ui->setupUi(this);

    // TODO infer type of item and get image from cache or generate one/grab icon
    // TODO setup appropriate click action based on item type
}

ItemButton::~ItemButton()
{
    delete ui;
}

}

}
