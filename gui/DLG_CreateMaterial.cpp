#include"DLG_CreateMaterial.h"
#include<QVBoxLayout>
#include<QPushButton>

ShaderUseWidget::ShaderUseWidget(const QString &name,QWidget *parent,QGridLayout *layout,int row,bool check)
{
    label=new QLabel(name,parent);
    edit=new QLineEdit(parent);
    browser=new QPushButton("Browser",parent);

    layout->addWidget(label,    row,1,Qt::AlignLeft);
    layout->addWidget(edit,     row,2);
    layout->addWidget(browser,  row,3);

    SetChecked(check);
}

void ShaderUseWidget::SetChecked(bool check)
{
    label->setEnabled(check);
    edit->setReadOnly(!check);
    edit->setEnabled(check);
    browser->setEnabled(check);
}

DLGCreateMaterial::DLGCreateMaterial()
{
    setWindowTitle("CreateMaterial");

    QWidget *widget=new QWidget(this);

    QVBoxLayout *layout=new QVBoxLayout(widget);

    {
        QWidget *grid_widget=new QWidget(widget);
        QGridLayout *grid_layout=new QGridLayout(grid_widget);

        {
            use_tess_shader=new QCheckBox(grid_widget);grid_layout->addWidget(use_tess_shader,1,0);connect(use_tess_shader,&QCheckBox::stateChanged,this,&DLGCreateMaterial::OnTessStateChanged);
            use_geom_shader=new QCheckBox(grid_widget);grid_layout->addWidget(use_geom_shader,3,0);connect(use_geom_shader,&QCheckBox::stateChanged,this,&DLGCreateMaterial::OnGeomStateChanged);
        }

        {
            vertex      =new ShaderUseWidget("vertex shader",                   grid_widget,grid_layout,0,true );connect(vertex       ->browser,&QPushButton::clicked,this,&DLGCreateMaterial::OnVertexBrowserClicked);
            tess_control=new ShaderUseWidget("tessellation control shader",     grid_widget,grid_layout,1,false);connect(tess_control ->browser,&QPushButton::clicked,this,&DLGCreateMaterial::OnTessControlBrowserClicked);
            tess_eval   =new ShaderUseWidget("tessellation evaluation shader",  grid_widget,grid_layout,2,false);connect(tess_eval    ->browser,&QPushButton::clicked,this,&DLGCreateMaterial::OnTessEvalBrowserClicked);
            geometry    =new ShaderUseWidget("geometry shader",                 grid_widget,grid_layout,3,false);connect(geometry     ->browser,&QPushButton::clicked,this,&DLGCreateMaterial::OnGeometryBrowserClicked);
            fragment    =new ShaderUseWidget("fragment shader",                 grid_widget,grid_layout,4,true );connect(fragment     ->browser,&QPushButton::clicked,this,&DLGCreateMaterial::OnFragmentBrowserClicked);
        }

        layout->addWidget(grid_widget);
    }

    {
        QWidget *button_area_widget=new QWidget(widget);
        QHBoxLayout *button_area_layout=new QHBoxLayout(button_area_widget);

        //确定、取消按钮
        {
            QPushButton *ok_button=new QPushButton(button_area_widget);
            ok_button->setText("OK");
            connect(ok_button,&QPushButton::clicked,this,&DLGCreateMaterial::OnOKClicked);

            button_area_layout->addWidget(ok_button,0,Qt::AlignRight);

            QPushButton *cancel_button=new QPushButton(button_area_widget);
            cancel_button->setText("Cancel");
            connect(cancel_button,&QPushButton::clicked,this,&DLGCreateMaterial::OnCancelClicked);
            
            button_area_layout->addWidget(cancel_button,0,Qt::AlignLeft);
        }

        layout->addWidget(button_area_widget);
    }

    resize(layout->sizeHint());
    setModal(true);
}

void DLGCreateMaterial::OnTessStateChanged(int state)
{
    tess_control->SetChecked(state);
    tess_eval->SetChecked(state);
}

void DLGCreateMaterial::OnGeomStateChanged(int state)
{
    geometry->SetChecked(state);
}

void DLGCreateMaterial::OnVertexBrowserClicked()
{

}

void DLGCreateMaterial::OnTessControlBrowserClicked()
{

}

void DLGCreateMaterial::OnTessEvalBrowserClicked()
{}

void DLGCreateMaterial::OnGeometryBrowserClicked()
{}

void DLGCreateMaterial::OnFragmentBrowserClicked()
{}

void DLGCreateMaterial::OnCancelClicked()
{
    close();
}

void DLGCreateMaterial::OnOKClicked()
{

}
