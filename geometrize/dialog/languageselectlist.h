#pragma once

#include <memory>

#include <QListWidget>

namespace geometrize
{

namespace dialog
{

class LanguageSelectList : public QListWidget
{
    Q_OBJECT

public:
    LanguageSelectList(QWidget* parent);
    ~LanguageSelectList();

protected:
    void changeEvent(QEvent*) override;

private:
    class LanguageSelectListImpl;
    std::unique_ptr<LanguageSelectListImpl> d;
};

}

}
