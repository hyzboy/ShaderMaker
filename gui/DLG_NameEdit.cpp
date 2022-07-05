#include"DLG_NameEdit.h"
#include<QVBoxLayout>
#include<QPushButton>

DLGNameEdit::DLGNameEdit(const QString &tn,const QString &str)
{
    result=false;
    type_name=tn;

    setModal(true);
//    setWindowTitle(tr("New..."));

    QVBoxLayout *layout=new QVBoxLayout(this);

    {
        QLabel *lab=new QLabel(tr("Please enter a name for the ")+type_name,this);

        lab->setAlignment(Qt::AlignLeft);

        layout->addWidget(lab);
    }

    {
        name_edit=new QLineEdit(str,this);

        name_edit->setAlignment(Qt::AlignCenter);

        name_edit->setCursorPosition(0);

        layout->addWidget(name_edit);
    }

    {
        hint_label=new QLabel(this);

        hint_label->setAlignment(Qt::AlignCenter);

        layout->addWidget(hint_label);
    }
    
    //按钮区
    {
        QWidget *button_widget=new QWidget(this);
        QHBoxLayout *button_layout=new QHBoxLayout(button_widget);

        //确定、取消按钮
        {
            QPushButton *ok_button=new QPushButton(button_widget);
            ok_button->setText(tr("OK"));
            connect(ok_button,&QPushButton::clicked,this,&DLGNameEdit::OnOKClicked);

            button_layout->addWidget(ok_button,0,Qt::AlignRight);

            QPushButton *cancel_button=new QPushButton(button_widget);
            cancel_button->setText(tr("Cancel"));
            connect(cancel_button,&QPushButton::clicked,this,&DLGNameEdit::OnCancelClicked);
            
            button_layout->addWidget(cancel_button,0,Qt::AlignLeft);
        }

        layout->addWidget(button_widget);
    }

    resize(layout->sizeHint());
}

void DLGNameEdit::OnCancelClicked()
{
    close();
}