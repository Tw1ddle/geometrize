#pragma once

#include <memory>

#include <QWidget>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The LanguageSelectWidget class implements a widget for selecting and viewing the application language/locale.
 */
class LanguageSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LanguageSelectWidget(QWidget* parent);
    ~LanguageSelectWidget();

protected:
    void changeEvent(QEvent*) override;

private:
    class LanguageSelectWidgetImpl;
    std::unique_ptr<LanguageSelectWidgetImpl> d;
};

}

}
