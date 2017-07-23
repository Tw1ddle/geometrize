#include "templategrid.h"

#include <QString>

#include "chaiscript/chaiscript.hpp"

#include "common/searchpaths.h"
#include "common/util.h"
#include "dialog/templatebutton.h"
#include "layout/flowlayout.h"
#include "script/chaiscriptcreator.h"

namespace geometrize
{

namespace dialog
{

class TemplateGrid::TemplateGridImpl
{
public:
    TemplateGridImpl(TemplateGrid* pQ) : q{pQ}, m_layout{new layout::FlowLayout(24, 24, 24)}, m_templateLoader{geometrize::script::createDefaultEngine()}
    {
        q->setLayout(m_layout);
    }

    void loadTemplates()
    {
        const std::vector<std::string> paths{geometrize::searchpaths::getTemplateSearchPaths()};

        for(const std::string& path : paths) {
            const std::vector<std::string> templateFolders{util::getTemplateFoldersForPath(path)};

            for(const std::string& folder : templateFolders) {
                const bool result{addTemplateItem(QString::fromStdString(folder))};
                emit q->signal_templateLoaded(QString::fromStdString(folder), result);
            }
        }
    }

    void setItemFilter(const QString& filter)
    {
        if(filter.isEmpty()) {
            for(TemplateButton* const button : m_buttons) {
                button->show();
            }
            return;
        }

        for(TemplateButton* const button : m_buttons) {
            const QString name{QString::fromStdString(button->getTemplateManifest().getName())};
            const std::vector<std::string> tags{button->getTemplateManifest().getTags()};

            if(name.contains(filter, Qt::CaseInsensitive)) {
                button->show();
            } else {
                button->hide();
            }

            for(const std::string& tag : tags) {
                const QString qTag{QString::fromStdString(tag)};
                if(qTag.contains(filter, Qt::CaseInsensitive)) {
                    button->show();
                }
            }
        }
    }

    TemplateGridImpl operator=(const TemplateGridImpl&) = delete;
    TemplateGridImpl(const TemplateGridImpl&) = delete;
    ~TemplateGridImpl() = default;

private:
    bool addTemplateItem(const QString& templateFolder)
    {
        TemplateButton* item{new TemplateButton(m_templateLoader.get(), templateFolder)};
        m_layout->addWidget(item);
        m_buttons.push_back(item);
        return true;
    }

    TemplateGrid* q;
    layout::FlowLayout* m_layout;
    std::unique_ptr<chaiscript::ChaiScript> m_templateLoader;
    std::vector<TemplateButton*> m_buttons;
};

TemplateGrid::TemplateGrid(QWidget* parent) :
    QWidget(parent),
    d{std::make_unique<TemplateGrid::TemplateGridImpl>(this)}
{
}

void TemplateGrid::loadTemplates()
{
    d->loadTemplates();
}

void TemplateGrid::setItemFilter(const QString& filter)
{
    d->setItemFilter(filter);
}

}

}
