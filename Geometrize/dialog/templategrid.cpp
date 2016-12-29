#include "templategrid.h"

#include <assert.h>

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
    TemplateGridImpl(TemplateGrid* pQ) : q{pQ}, m_layout{new layout::FlowLayout(40, 32, 32)}, m_templateLoader{geometrize::script::createChaiScript()}
    {
        q->setLayout(m_layout);

        const std::string basePath{geometrize::searchpaths::getApplicationDirectoryPath()};
        const std::vector<std::string> paths{geometrize::searchpaths::getTemplateSearchPaths()};
        for(const std::string& path : paths) {
            const QString fullPath{QString::fromStdString(basePath + path)};
            m_fileWatch.addPath(fullPath);

            const std::vector<std::string> templates{util::getTemplateFoldersForPath(fullPath.toStdString())};

            for(const std::string& t : templates) {
                const bool result{addTemplateItem(QString::fromStdString(t))};
                emit q->signal_templateLoaded(QString::fromStdString(t), result);
            }
        }
    }

    TemplateGridImpl operator=(const TemplateGridImpl&) = delete;
    TemplateGridImpl(const TemplateGridImpl&) = delete;
    ~TemplateGridImpl() = default;

private:
    bool addTemplateItem(const QString& templateFolder)
    {
        m_layout->addWidget(new TemplateButton(m_templateLoader.get(), templateFolder));
        return true;
    }

    TemplateGrid* q;
    layout::FlowLayout* m_layout;
    QFileSystemWatcher m_fileWatch;
    std::unique_ptr<chaiscript::ChaiScript> m_templateLoader;
};

TemplateGrid::TemplateGrid(QWidget* parent) :
    QWidget(parent),
    d{std::make_unique<TemplateGrid::TemplateGridImpl>(this)}
{
}

}

}
