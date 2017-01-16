#include "templategrid.h"

#include <assert.h>
#include <list>

#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "script/chaiscriptcreator.h"
#include "chaiscript/chaiscript.hpp"
#include "layout/flowlayout.h"
#include "searchpaths.h"
#include "dialog/templatebutton.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

class TemplateGrid::TemplateGridImpl
{
public:
    TemplateGridImpl(TemplateGrid* pQ) : q{pQ}, m_layout{new layout::FlowLayout(24, 24, 24)}, m_templateLoader{geometrize::script::createChaiScript()}
    {
        q->setLayout(m_layout);
    }

    void loadTemplates()
    {
        const std::string basePath{geometrize::searchpaths::getApplicationDirectoryPath()};
        const std::vector<std::string> paths{geometrize::searchpaths::getTemplateSearchPaths()};
        for(const std::string& path : paths) {
            const QString fullPath{QString::fromStdString(basePath + path)};
            m_fileWatch.addPath(fullPath);

            const std::vector<std::string> templateFolders{util::getTemplateFoldersForPath(fullPath.toStdString())};

            for(const std::string& folder : templateFolders) {
                const bool result{addTemplateItem(QString::fromStdString(folder))};
                emit q->signal_templateLoaded(QString::fromStdString(folder), result);
            }
        }
    }

    // TODO remove this and use a modified grid layout instead
    void setItemFilter(const QString& filter)
    {
        const std::string stdFilter{filter.toStdString()};
        for(TemplateButton* const button : m_buttons) {
            if(util::stringBeginsWith(button->getTemplateManifest().getName(), stdFilter)) {
                button->show();
            } else {
                button->hide();
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
    QFileSystemWatcher m_fileWatch;
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
