#pragma once

#include <QWidget>

namespace Ui
{
class ItemButton;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ItemButton class is a button that opens a recent job when pressed, like a recently opened image.
 */
class ItemButton : public QWidget
{
    Q_OBJECT

public:
    explicit ItemButton(const QString& item);
    ~ItemButton();

private:
    Ui::ItemButton* ui;
    const QString m_item;
};

}

}
