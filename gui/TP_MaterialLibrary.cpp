#include"TP_MaterialLibrary.h"
#include<QVBoxLayout>
#include<QSplitter>
#include<QTextEdit>
#include<hgl/filesystem/EnumFile.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
#include"ConfigData.h"

using namespace hgl;
using namespace hgl::filesystem;

#define TYPE_MATERIAL   "Material"
#define TYPE_VERTEX     "Vertex"
#define TYPE_GEOMETRY   "Geometry"
#define TYPE_FRAGMENT   "Fragment"

#define COLUMN_NAME     0
#define COLUMN_TYPE     1
#define COLUMN_FILENAME 2

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

TPMaterialLibrary::TPMaterialLibrary()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(this);

    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(splitter);

    //左侧文件树列表
    {
        file_tree_widget=new QTreeWidget(splitter);

        current_item=nullptr;

        UpdateFileTree();

        connect(file_tree_widget,&QTreeWidget::itemDoubleClicked,this,&TPMaterialLibrary::OnFileChanged);
    }

    //右侧编辑区
    {
        QSplitter *right_splitter=new QSplitter(Qt::Vertical,splitter);

        {
            glsl_editor=new GLSLTextEdit(right_splitter);

            glsl_editor->setReadOnly(false);

            connect(glsl_editor,&QTextEdit::cursorPositionChanged,this,&TPMaterialLibrary::OnCursorPositionChanged);
            connect(glsl_editor,&QTextEdit::textChanged,this,&TPMaterialLibrary::OnTextChanged);
        }

        {
            QWidget *build_widget=new QWidget(right_splitter);
            QHBoxLayout *build_layout=new QHBoxLayout(build_widget);

            build_layout->setContentsMargins(0,0,0,0);

            {
                save_button=new QPushButton("Save",build_widget);                
                build_layout->addWidget(save_button,0,Qt::AlignLeft);
                connect(save_button,&QPushButton::clicked,this,&TPMaterialLibrary::OnSave);

                convert_to_glsl_button=new QPushButton("Convert to GLSL",build_widget);
                build_layout->addWidget(convert_to_glsl_button,0,Qt::AlignLeft);

                convert_and_compile_button=new QPushButton("Convert&&Compile",build_widget);
                build_layout->addWidget(convert_and_compile_button,0,Qt::AlignLeft);

                build_button=new QPushButton("Build",build_widget);                
                build_layout->addWidget(build_button,0,Qt::AlignLeft);

                build_layout->addStretch();

                editor_hint=new QLabel(build_widget);
                build_layout->addWidget(editor_hint,0,Qt::AlignRight);
            }

            build_widget->setFixedHeight(build_widget->height());
        }

        {
            log_text=new QTextEdit(right_splitter);

            log_text->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            log_text->setFrameShape(QFrame::StyledPanel);
            log_text->setReadOnly(true);
            log_text->setLineWrapMode(QTextEdit::NoWrap);
            log_text->setTabStopWidth(4);
        }

        right_splitter->setStretchFactor(0,5);
        right_splitter->setStretchFactor(1,0);
        right_splitter->setStretchFactor(2,2);
    }

    splitter->setStretchFactor(0,2);
    splitter->setStretchFactor(1,5);
    
    layout->addWidget(splitter);
}

TPMaterialLibrary::~TPMaterialLibrary()
{
}

void TPMaterialLibrary::OnFileChanged(QTreeWidgetItem *item,int)
{
    if(current_item==item)return;

    //初始化
    {
        glsl_editor->clear();
        editor_hint->clear();
        log_text->clear();

        save_button->setEnabled(false);
        build_button->setEnabled(true);

        current_item=item;

        text_changed=false;
    }

    UTF8StringList sl;

    filename=ToOSString(item->text(COLUMN_FILENAME));
    if(LoadStringListFromTextFile(sl,filename)<=0)
        return;

    const UTF8String str=ToString(sl,UTF8String("\n"));

    glsl_editor->setText(ToQString(str));
    
    save_button->setEnabled(false);
    text_changed=false;
    
}

void TPMaterialLibrary::OnCursorPositionChanged()
{
    const QTextCursor cursor=glsl_editor->textCursor();

    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    glsl_editor->setExtraSelections(extraSelections);

    editor_hint->setText("Ln: "+QString::number(cursor.blockNumber()+1)
                    +"    Ch: "+QString::number(cursor.columnNumber()+1)
                    +"    ");
}

void TPMaterialLibrary::OnTextChanged()
{
    text_changed=true;

    save_button->setEnabled(true);
}

void TPMaterialLibrary::OnSave()
{
    if(!text_changed)return;

    const QString text=glsl_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    filesystem::SaveMemoryToFile(filename,u8text.c_str(),u8text.Length());

    text_changed=false;
    save_button->setEnabled(false);
}