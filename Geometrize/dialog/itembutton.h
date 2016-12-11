#pragma once

#include <QWidget>

namespace Ui {
class ItemButton;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ItemButton class is a button that represents an item, like a recently opened job.
 */
class ItemButton : public QWidget
{
    Q_OBJECT

public:
    explicit ItemButton(const QString& item);
    ~ItemButton();

private:
    Ui::ItemButton *ui;

    const QString m_item;
};

}

}
