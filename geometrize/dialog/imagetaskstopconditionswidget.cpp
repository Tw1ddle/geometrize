#include "imagetaskstopconditionswidget.h"
#include "ui_imagetaskstopconditionswidget.h"

#include <cassert>
#include <cstdint>
#include <memory>

#include <QEvent>
#include <QList>
#include <QLocale>
#include <QMessageBox>
#include <QString>

#include "chaiscript/chaiscript.hpp"

#include "dialog/scripteditorwidget.h"
#include "script/chaiscriptcreator.h"

namespace geometrize
{

namespace dialog
{

void showImageTaskStopConditionMetMessage(QWidget* parent)
{
    QMessageBox::information(parent, QObject::tr("Stop Condition Met"), QObject::tr("Stop condition for geometrizing was met"));
}

class ImageTaskStopConditionsWidget::ImageTaskStopConditionsWidgetImpl
{
public:
    ImageTaskStopConditionsWidgetImpl(ImageTaskStopConditionsWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageTaskStopConditionsWidget>()}, m_stopConditionId{0}, m_engine{createEngine()}
    {
        ui->setupUi(q);
        populateUi();

        connect(ui->addStopConditionButton, &QPushButton::clicked, [this]() {
            const std::string editorName = tr("Custom Stop Condition").toStdString();

            const std::string functionName = "stop_condition_" + std::to_string(m_stopConditionId++);

            std::string defaultCode = "def ";
            defaultCode.append(functionName);
            defaultCode.append("() { return shapeCount <= shapeLimit; }");

            auto widget = new geometrize::dialog::ScriptEditorWidget(editorName, functionName, defaultCode);
            ui->scriptEditorLayout->addWidget(widget);
        });
        connect(ui->clearStopConditionsButton, &QPushButton::clicked, [this]() {
            QLayoutItem* item = nullptr;
            while((item = ui->scriptEditorLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }

            m_stopConditionId = 0;
        });
    }
    ~ImageTaskStopConditionsWidgetImpl() = default;
    ImageTaskStopConditionsWidgetImpl operator=(const ImageTaskStopConditionsWidgetImpl&) = delete;
    ImageTaskStopConditionsWidgetImpl(const ImageTaskStopConditionsWidgetImpl&) = delete;

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

    bool stopConditionsMet(const std::size_t currentShapeCount) const
    {
        const std::size_t shapeLimit = static_cast<std::size_t>(ui->shapeCountConditionSpinBox->value());

        const bool enableShapeLimitCheck = ui->shapeCountConditionEnabledCheckbox->isChecked();
        if(enableShapeLimitCheck && currentShapeCount >= shapeLimit) {
            return true;
        }

        const auto scriptWidgets = ui->scriptEditorLayout->findChildren<geometrize::dialog::ScriptEditorWidget*>();

        bool scriptStopConditionMet = false;
        for(const auto& widget : scriptWidgets) {
            try {
                auto stopCondition = m_engine->eval<std::function<bool()>>(widget->getFunctionName());
                if(stopCondition()) {
                    scriptStopConditionMet = true;
                }
            } catch(const chaiscript::exception::eval_error& e) {
                widget->onScriptEvaluationFailed(e.pretty_print());
            } catch(...) {
                widget->onScriptEvaluationFailed("Unknown script evaluation error");
            }
            widget->onScriptEvaluationSucceeded();
        }

        return scriptStopConditionMet;
    }

private:
    std::unique_ptr<chaiscript::ChaiScript> createEngine()
    {
        return geometrize::script::createImageTaskEngine();
    }

    void populateUi()
    {
    }

    ImageTaskStopConditionsWidget* q;
    std::unique_ptr<Ui::ImageTaskStopConditionsWidget> ui;

    int m_stopConditionId;
    std::unique_ptr<chaiscript::ChaiScript> m_engine;
};

ImageTaskStopConditionsWidget::ImageTaskStopConditionsWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskStopConditionsWidget::ImageTaskStopConditionsWidgetImpl>(this)}
{
}

ImageTaskStopConditionsWidget::~ImageTaskStopConditionsWidget()
{
}

void ImageTaskStopConditionsWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

bool ImageTaskStopConditionsWidget::stopConditionsMet(const std::size_t currentShapeCount) const
{
    return d->stopConditionsMet(currentShapeCount);
}

}

}
