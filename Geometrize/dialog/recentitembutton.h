#ifndef RECENTITEMBUTTON_H
#define RECENTITEMBUTTON_H

#include <QWidget>

namespace Ui {
class RecentItemButton;
}

namespace geometrize
{

namespace dialog
{

class RecentItemButton : public QWidget
{
    Q_OBJECT

public:
    explicit RecentItemButton(const QString& item);
    ~RecentItemButton();

private:
    Ui::RecentItemButton *ui;

    const QString m_item;
};

}

}

#endif // RECENTITEMBUTTON_H
