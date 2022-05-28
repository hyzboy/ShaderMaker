#include"TP_MaterialLibrary.h"
#include"ConfigData.h"
#include<hgl/filesystem/EnumFile.h>
#include<hgl/type/QTString.h>
#include"WI_EditorTreeWidgetItem.h"
#include<QStringList>

using namespace hgl;
using namespace hgl::filesystem;

constexpr float MLIColor[(size_t)MaterialFileType::RANGE_SIZE][3]=
{
    {0,0,0},
    
    {64,64,255},
    {64,128,128},
    {255,64,64},
    {128,128,64}
};

EditorTreeWidgetItem *CreateFileItem(QTreeWidgetItem *parent,const QString &name,const MaterialFileType &type,const QString &type_name,const FileInfo *fi)
{
    QStringList header;

    header<<name<<type_name;

    EditorTreeWidgetItem *item=new EditorTreeWidgetItem(parent,header,fi,type);

    QColor color;

    if(RangeCheck(type))
    {
        color.setRgbF(MLIColor[(size_t)type][0]/255.0f,
                      MLIColor[(size_t)type][1]/255.0f,
                      MLIColor[(size_t)type][2]/255.0f);
    }
    else
        color=item->textColor(0);
    
    for(int i=0;i<item->columnCount();i++)
        item->setTextColor(i,color);

    return item;
}

class EnumMaterialFileConfig:public EnumFileConfig
{
public:

    QTreeWidgetItem *node;

public:

    EnumMaterialFileConfig(QTreeWidgetItem *ri,const OSString &path):EnumFileConfig(path)
    {
        node=ri;
    }

    EnumMaterialFileConfig(QTreeWidgetItem *ri,const EnumFileConfig *efc,const OSString &sub_folder_name):EnumFileConfig(efc,sub_folder_name)
    {
        node=ri;
    }
};//

class EnumMaterialFile:public EnumFile
{
public:

    EnumFileConfig *CreateSubConfig(struct EnumFileConfig *up_efc,const FileInfo &fi) override
    {
        EnumMaterialFileConfig *emfc=(EnumMaterialFileConfig *)up_efc;

        const OSString full_sub_folder_name=MergeFilename(up_efc->folder_name,fi.name);

        QTreeWidgetItem *sub_node=new EditorTreeWidgetItem(emfc->node,QStringList(ToQString(fi.name)),&fi);

        emfc->node->addChild(sub_node);

        return(new EnumMaterialFileConfig(sub_node,up_efc,full_sub_folder_name));
    }
    
    void ProcFile(EnumFileConfig *efc,FileInfo &fi) override
    {
        EnumMaterialFileConfig *emfc=(EnumMaterialFileConfig *)efc;

        const OSString filename=OSString(fi.name);

        const OSString ext_name=ClipFileExtName(filename,false);

        if(ext_name.CaseComp(OS_TEXT("xml"))!=0)return;

        const OSString s_fn=ClipFileMainname(filename);

        const OSString type_name=ClipFileExtName(s_fn,false);

        const QString main_name=ToQString(ClipFileMainname(s_fn));

        QTreeWidgetItem *item=nullptr;

        #define CONFIRM_FILE_TYPE(name) if(type_name.CaseComp(OS_TEXT(#name))==0)  \
                                            item=CreateFileItem(emfc->node,main_name,MaterialFileType::##name,#name,&fi); \
                                        else

        CONFIRM_FILE_TYPE(Material)
        CONFIRM_FILE_TYPE(Vert)
        CONFIRM_FILE_TYPE(Geom)
        CONFIRM_FILE_TYPE(Frag)
        {
            //...未知
        }
        #undef CONFIRM_FILE_TYPE

        emfc->node->addChild(item);
    }
};//

void TPMaterialLibrary::UpdateFileTree()
{
    QStringList header;

    header<<"name"<<"type";

    file_tree_widget->setFont(GetUIFont());

    file_tree_widget->clear();
    file_tree_widget->setHeaderLabels(header);
    
    QTreeWidgetItem *root_item=new EditorTreeWidgetItem(nullptr,QStringList("root"));

    {
        EnumMaterialFileConfig efc(root_item,GetMaterialSourcePath());

        efc.proc_file=true;
        efc.sub_folder=true;

        EnumMaterialFile ef;

        ef.Enum(&efc);
    }

    file_tree_widget->addTopLevelItem(root_item);

    file_tree_widget->expandItem(root_item);    
    file_tree_widget->resizeColumnToContents(0);
    file_tree_widget->resizeColumnToContents(1);

    file_tree_widget->setMinimumWidth(file_tree_widget->columnWidth(0)+
                                      file_tree_widget->columnWidth(1));
}