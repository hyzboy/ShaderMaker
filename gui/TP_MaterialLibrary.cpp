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
        save_button->setHidden(true);
        build_button->setEnabled(true);

        current_item=item;

        text_changed=false;
    }

    UTF8StringList sl;

    filename=ToOSString(item->text(ML_COLUMN_FILENAME));
    if(LoadStringListFromTextFile(sl,filename)<=0)
        return;

    const UTF8String str=ToString(sl,UTF8String("\n"));

    glsl_editor->setText(ToQString(str));

    //setText后文件会改变，需重设置这些值
    save_button->setEnabled(false);
    save_button->setHidden(true);
    text_changed=false;
    
    const QString type=item->text(ML_COLUMN_TYPE);

    if(type==ML_TYPE_MATERIAL)
    {
        convert_to_glsl_button->setEnabled(false);
        convert_to_glsl_button->setHidden(true);
        convert_and_compile_button->setEnabled(false);
        convert_and_compile_button->setHidden(true);

        build_button->setEnabled(true);
        build_button->setHidden(false);
    }
    else
    {
        convert_to_glsl_button->setEnabled(true);
        convert_to_glsl_button->setHidden(false);
        convert_and_compile_button->setEnabled(true);
        convert_and_compile_button->setHidden(false);
        build_button->setEnabled(false);
        build_button->setHidden(true);
    }
}
