#include"TP_MaterialLibrary.h"
#include"ConfigData.h"
#include<hgl/filesystem/EnumFile.h>
#include<hgl/type/QTString.h>

using namespace hgl;
using namespace hgl::filesystem;

#define TYPE_MATERIAL   "Material"
#define TYPE_VERTEX     "Vertex"
#define TYPE_GEOMETRY   "Geometry"
#define TYPE_FRAGMENT   "Fragment"

QTreeWidgetItem *CreateFileItem(QTreeWidgetItem *parent,const QString &name,const QString &type="",const QString &fullname="")
{
    QTreeWidgetItem *item=new QTreeWidgetItem(parent);

    item->setText(COLUMN_NAME,name);

    if(type=="folder")
    {
    }
    else
    {
        item->setText(COLUMN_TYPE,type);

        QColor color;

        if(type==TYPE_MATERIAL  )color=QColor( 64, 64,255);else
        if(type==TYPE_VERTEX    )color=QColor(255, 64, 64);else
        if(type==TYPE_GEOMETRY  )color=QColor( 64,128,128);else
        if(type==TYPE_FRAGMENT  )color=QColor(128,128, 64);else
            color=item->textColor(0);
        
        item->setTextColor(COLUMN_NAME,color);
        item->setTextColor(COLUMN_TYPE,color);
    }

    item->setText(COLUMN_FILENAME,fullname);

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

        QTreeWidgetItem *sub_node=CreateFileItem(emfc->node,ToQString(fi.name),"sub_folder");

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

        const OSString main_name=ClipFileMainname(s_fn);

        QTreeWidgetItem *item=nullptr;

        #define CONFIRM_FILE_TYPE(name,short_name)  if(type_name.CaseComp(OS_TEXT(short_name))==0)  \
                                                        item=CreateFileItem(emfc->node,ToQString(main_name),TYPE_##name,ToQString(fi.fullname)); \
                                                    else

        CONFIRM_FILE_TYPE(MATERIAL, "Material")
        CONFIRM_FILE_TYPE(VERTEX,   "vert")
        CONFIRM_FILE_TYPE(GEOMETRY, "geom")
        CONFIRM_FILE_TYPE(FRAGMENT, "frag")
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

    header<<"name"<<"type";//<<"full filename";     //最后一列隐藏吧！

    file_tree_widget->clear();
    file_tree_widget->setHeaderLabels(header);

    QTreeWidgetItem *root_item=CreateFileItem(nullptr,"root");

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
}