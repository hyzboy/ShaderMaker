#include"TP_ShaderFile.h"
#include"ConfigData.h"
#include<hgl/filesystem/EnumFile.h>
#include<hgl/type/QTString.h>
#include"TypeDefine.h"

using namespace hgl;
using namespace hgl::filesystem;

namespace
{
    QTreeWidgetItem *CreateFileItem(QTreeWidgetItem *parent,const QString &name,bool is_folder,const QString &fullname="")
    {
        QTreeWidgetItem *item=new QTreeWidgetItem(parent);

        item->setText((int)ShaderFileColumn::Name,     name);
        item->setText((int)ShaderFileColumn::Filename, fullname);

        return item;
    }

    class EnumShaderModuleFileConfig:public EnumFileConfig
    {
    public:

        QTreeWidgetItem *node;

    public:

        EnumShaderModuleFileConfig(QTreeWidgetItem *ri,const OSString &path):EnumFileConfig(path)
        {
            node=ri;
        }

        EnumShaderModuleFileConfig(QTreeWidgetItem *ri,const EnumFileConfig *efc,const OSString &sub_folder_name):EnumFileConfig(efc,sub_folder_name)
        {
            node=ri;
        }
    };//

    class EnumMaterialFile:public EnumFile
    {
    public:

        EnumFileConfig *CreateSubConfig(struct EnumFileConfig *up_efc,const FileInfo &fi) override
        {
            EnumShaderModuleFileConfig *emfc=(EnumShaderModuleFileConfig *)up_efc;

            const OSString full_sub_folder_name=MergeFilename(up_efc->folder_name,fi.name);

            QTreeWidgetItem *sub_node=CreateFileItem(emfc->node,ToQString(fi.name),true);

            emfc->node->addChild(sub_node);

            return(new EnumShaderModuleFileConfig(sub_node,up_efc,full_sub_folder_name));
        }
    
        void ProcFile(EnumFileConfig *efc,FileInfo &fi) override
        {
            EnumShaderModuleFileConfig *emfc=(EnumShaderModuleFileConfig *)efc;

            const OSString filename=OSString(fi.name);

            const OSString ext_name=ClipFileExtName(filename,false);

            if(ext_name.CaseComp(OS_TEXT("xml"))!=0)return;

            const OSString s_fn=ClipFileMainname(filename);

            const OSString type_name=ClipFileExtName(s_fn,false);

            const QString main_name=ToQString(ClipFileMainname(s_fn));
            const QString fullname=ToQString(fi.fullname);

            QTreeWidgetItem *item=nullptr;
            
            item=CreateFileItem(emfc->node,main_name,false,fullname);

            emfc->node->addChild(item);
        }
    };//
}//namespace

void TPShaderFile::UpdateFileTree()
{
    QStringList header;

    header<<"name";//<<"full filename";     //最后一列隐藏

    file_tree_widget->clear();
    file_tree_widget->setHeaderLabels(header);

    QTreeWidgetItem *root_item=CreateFileItem(nullptr,"root",true);

    {
        EnumShaderModuleFileConfig efc(root_item,GetMaterialSourcePath());

        efc.proc_file=true;
        efc.sub_folder=true;

        EnumMaterialFile ef;

        ef.Enum(&efc);
    }

    file_tree_widget->addTopLevelItem(root_item);

    file_tree_widget->expandItem(root_item);
    file_tree_widget->resizeColumnToContents(0);
    file_tree_widget->resizeColumnToContents(1);
}
