#include"TP_MaterialLibrary.h"
#include<QVBoxLayout>
#include<QSplitter>
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
#include"W_XMLShaderEditor.h"

using namespace hgl;

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
        editor_tab_widget=new QTabWidget(splitter);
        editor_tab_widget->resize(QSize(width()*0.8,height()));
        editor_tab_widget->setTabsClosable(true);
        editor_tab_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

        connect(editor_tab_widget,&QTabWidget::tabCloseRequested,this,&TPMaterialLibrary::OnTabCloseRequested);
    }

    layout->addWidget(splitter);
}

void TPMaterialLibrary::OnFileChanged(QTreeWidgetItem *item,int)
{
    if(current_item==item)return;

    for(int i=0;i<editor_tab_widget->count();i++)
    {
        EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(i);

        if(w->GetItem()==item)
        {
            editor_tab_widget->setCurrentIndex(i);
            return;
        }
    }

    const OSString &filename=ToOSString(item->text(ML_COLUMN_FILENAME));

    const QString type=item->text(ML_COLUMN_TYPE);

    if(type==ML_TYPE_MATERIAL)
    {

    }
    else
    {
        XMLShaderEditorWidget *xs_editor=new XMLShaderEditorWidget(item,filename);

        const int index=editor_tab_widget->addTab(xs_editor,item->text(ML_COLUMN_TYPE)+":"+
                                                            item->text(ML_COLUMN_NAME));

        editor_tab_widget->setCurrentIndex(index);
    }
}

void TPMaterialLibrary::OnTabCloseRequested(int index)
{
    EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(index);

    if(!w)return;

    if(w->OnCloseRequested())
    {
        editor_tab_widget->removeTab(index);
    }
}