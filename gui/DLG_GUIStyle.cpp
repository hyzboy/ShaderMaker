#include"DLG_GUIStyle.h"
#include<QVBoxLayout>
#include<QApplication>
#include<QStyle>
#include<QStyleFactory>
#include<QRadioButton>
#include<QButtonGroup>
#include"ConfigData.h"
#include<hgl/qt/Style.h>

DLGGUIStyle::DLGGUIStyle()
{
    setModal(true);
    setWindowTitle(tr("GUI Style"));

    QVBoxLayout *layout=new QVBoxLayout(this);
    
    style_list=hgl::qt::GetStyleList();
    choose_style=QApplication::style()->objectName();

    {
        QButtonGroup *group=new QButtonGroup(this);

        for(int i=0;i<style_list.count();i++)
        {
            QRadioButton *radio_button=new QRadioButton(style_list[i],this);

            group->addButton(radio_button,i);

            if(choose_style.compare(style_list[i],Qt::CaseInsensitive)==0)
                radio_button->setChecked(true);
            
            layout->addWidget(radio_button);
        }
        
        connect(group,&QButtonGroup::idClicked,this,&DLGGUIStyle::OnStyleChange);
    }

    connect(this,&QDialog::finished,this,&DLGGUIStyle::OnFinished);

    resize(layout->sizeHint());
}

void DLGGUIStyle::OnStyleChange(int index)
{
    hgl::qt::SetApplicationStyle(style_list[index]);
}

void DLGGUIStyle::OnFinished(int)
{
    SaveConfigData();
}