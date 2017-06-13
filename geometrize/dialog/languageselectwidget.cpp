#include "languageselectwidget.h"
#include "ui_languageselectwidget.h"

namespace geometrize
{

namespace dialog
{

class LanguageSelectWidget::LanguageSelectWidgetImpl
{
public:
    LanguageSelectWidgetImpl(LanguageSelectWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::LanguageSelectWidget>()}
    {
        ui->setupUi(q);
    }

    LanguageSelectWidgetImpl operator=(const LanguageSelectWidgetImpl&) = delete;
    LanguageSelectWidgetImpl(const LanguageSelectWidgetImpl&) = delete;
    ~LanguageSelectWidgetImpl()
    {
    }

private:
    LanguageSelectWidget* q;
    std::unique_ptr<Ui::LanguageSelectWidget> ui;
};

LanguageSelectWidget::LanguageSelectWidget(QWidget* parent) : QWidget(parent), d{std::make_unique<LanguageSelectWidget::LanguageSelectWidgetImpl>(this)}
{
}

LanguageSelectWidget::~LanguageSelectWidget()
{
}

void LanguageSelectWidget::changeEvent(QEvent* event)
{
}

}

}
