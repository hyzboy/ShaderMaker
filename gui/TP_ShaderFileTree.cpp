#include"TP_ShaderFile.h"
#include"ConfigData.h"
#include<hgl/filesystem/EnumFile.h>
#include<hgl/type/QTString.h>
#include"TypeDefine.h"
#include"WI_EditorTreeWidgetItem.h"

using namespace hgl;
using namespace hgl::filesystem;

namespace
{
    class EnumShaderFileConfig:public EnumFileConfig
    {
    public:

        QTreeWidgetItem *node;

    public:

        EnumShaderFileConfig(QTreeWidgetItem *ri,const OSString &path):EnumFileConfig(path)
        {
            node=ri;
        }

        EnumShaderFileConfig(QTreeWidgetItem *ri,const EnumFileConfig *efc,const OSString &sub_folder_name):EnumFileConfig(efc,sub_folder_name)
        {
            node=ri;
        }
    };//

    class EnumShaderFile:public EnumFile
    {
    public:

        EnumFileConfig *CreateSubConfig(struct EnumFileConfig *up_efc,const FileInfo &fi) override
        {
            EnumShaderFileConfig *emfc=(EnumShaderFileConfig *)up_efc;

            const OSString full_sub_folder_name=MergeFilename(up_efc->folder_name,fi.name);

            QTreeWidgetItem *sub_node=new EditorTreeWidgetItem(emfc->node,QStringList(ToQString(fi.name)),&fi);

            emfc->node->addChild(sub_node);

            return(new EnumShaderFileConfig(sub_node,up_efc,full_sub_folder_name));
        }
    
        void ProcFile(EnumFileConfig *efc,FileInfo &fi) override
        {
            EnumShaderFileConfig *emfc=(EnumShaderFileConfig *)efc;

            const OSString filename=OSString(fi.name);

            const OSString ext_name=ClipFileExtName(filename,false);

            if(ext_name.CaseComp(OS_TEXT("glsl"))!=0)return;

            const OSString s_fn=ClipFileMainname(filename);

            QTreeWidgetItem *item=new EditorTreeWidgetItem(emfc->node,QStringList(ToQString(s_fn)),&fi);

            emfc->node->addChild(item);
        }
    };//
}//namespace

void TPShaderFile::UpdateFileTree()
{
    QStringList header;

    header<<"Shader Name";

    file_tree_widget->clear();
    file_tree_widget->setHeaderLabels(header);
    file_tree_widget->setHeaderHidden(true);

    QTreeWidgetItem *root_item=new EditorTreeWidgetItem(nullptr,QStringList("root"),nullptr);

    {
        EnumShaderFileConfig efc(root_item,GetShaderLibraryPath());

        efc.proc_file=true;
        efc.sub_folder=true;

        EnumShaderFile ef;

        ef.Enum(&efc);
    }

    file_tree_widget->addTopLevelItem(root_item);

    file_tree_widget->expandItem(root_item);
    file_tree_widget->resizeColumnToContents(0);
    file_tree_widget->resizeColumnToContents(1);

    file_tree_widget->setMinimumWidth(file_tree_widget->columnWidth(0));
}