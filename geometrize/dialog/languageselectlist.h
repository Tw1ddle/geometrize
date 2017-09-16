#pragma once

#include <memory>

#include <QListWidget>

class QEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The LanguageSelectList class implements a widget for selecting the application language/locale from a list of options.
 */
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
