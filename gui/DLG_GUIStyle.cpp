#include"DLG_GUIStyle.h"
#include<QVBoxLayout>
#include<QApplication>
#include<QStyle>
#include<QStyleFactory>
#include<QRadioButton>
#include<QButtonGroup>
#include"ConfigData.h"

namespace hgl{namespace qt{
    const QStringList GetExtraGUIStyleList();
    QStyle *CreateQTExtraStyle(const QString &style_name);
}}

DLGGUIStyle::DLGGUIStyle()
{
    setModal(true);
    setWindowTitle(tr("GUI Style"));

    QVBoxLayout *layout=new QVBoxLayout(this);
    
    style_list=QStyleFactory::keys();
    choose_style=QApplication::style()->objectName();

    extra_style_list=hgl::qt::GetExtraGUIStyleList();

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

        for(int i=0;i<extra_style_list.count();i++)
        {
            QRadioButton *radio_button=new QRadioButton(extra_style_list[i],this);

            group->addButton(radio_button,i+style_list.count());

            if(choose_style.compare(extra_style_list[i],Qt::CaseInsensitive)==0)
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
    if(index<style_list.count())
    {
        QApplication::setStyle(style_list[index]);
    }
    else
    {
        index-=style_list.count();

        QStyle *s=hgl::qt::CreateQTExtraStyle(extra_style_list[index]);

        if(!s)
        {
            OnStyleChange(0);
            return;
        }

        QApplication::setStyle(s);
    }
}

void DLGGUIStyle::OnFinished(int)
{
    SaveConfigData();
}