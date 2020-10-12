#include"TP_MaterialLibrary.h"
#include<QVBoxLayout>
#include<QSplitter>
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>

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
    InitEditor(splitter);

    splitter->setStretchFactor(0,2);
    splitter->setStretchFactor(1,5);
    
    layout->addWidget(splitter);
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
