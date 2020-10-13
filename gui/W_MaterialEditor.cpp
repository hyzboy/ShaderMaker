#include"W_MaterialEditor.h"
#include<QVBoxLayout>
#include<QGridLayout>
#include"ShaderLib.h"
#include<hgl/type/QTString.h>

using namespace shader_lib;

const char *shader_type_name[]=
{
    "Vertex",
    "Tessellation Control",
    "Tessellation Evaluation",
    "Geometry",
    "Fragment"
};

const ShaderStageBits shader_bits_list[]=
{
    ssbVertex,
    ssbTesc,
    ssbTesv,
    ssbGeometry,
    ssbFragment
};

MaterialEditorWidget::MaterialEditorWidget(QTreeWidgetItem *i,const OSString &fn):EditorWidget(i,fn)
{
    QVBoxLayout *v_layout=new QVBoxLayout(this);

    //shadert选择区
    {
        QWidget *shader_widget=new QWidget(this);
        {
            QGridLayout *grid_layout=new QGridLayout(shader_widget);

            tess_check=new QCheckBox(shader_widget);
            geom_check=new QCheckBox(shader_widget);

            grid_layout->addWidget(tess_check,1,0);
            grid_layout->addWidget(geom_check,3,0);

            for(int i=0;i<5;i++)
            {
                sea[i].name_label=new QLabel(shader_widget);
                sea[i].name_label->setText(shader_type_name[i]);

                sea[i].fn_edit=new QLineEdit(shader_widget);

                sea[i].file_browser=new QPushButton(shader_widget);
                sea[i].file_browser->setText("Browser");
                sea[i].open_file=new QPushButton(shader_widget);
                sea[i].open_file->setText("Open");

                grid_layout->addWidget(sea[i].name_label,   i,1);
                grid_layout->addWidget(sea[i].fn_edit,      i,2);
                grid_layout->addWidget(sea[i].file_browser, i,3);
                grid_layout->addWidget(sea[i].open_file,    i,4);
            }
        }

        v_layout->addWidget(shader_widget);
    }

    //按钮区
    {
        QWidget *button_area=new QWidget(this);

        {
            QHBoxLayout *h_layout=new QHBoxLayout(button_area);

            build=new QPushButton(button_area);
            build->setText("Build");
            h_layout->addWidget(build);

            preview=new QPushButton(button_area);
            preview->setText("Preview");
            h_layout->addWidget(preview);

            h_layout->addStretch(0);
        }

        v_layout->addWidget(button_area);
    }

    //log区
    {
        log_widget=new QPlainTextEdit(this);
        log_widget->setReadOnly(true);

        log_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

        v_layout->addWidget(log_widget);
    }

    //load shader
    {
        InfoOutput *info_out=GetDefaultInfoOutput();

        XMLMaterial *xm=LoadXMLMaterial(GetFilename(),info_out);

        if(!xm)
            return;

        for(int i=0;i<5;i++)
        {
            ShaderStageBits ssb=shader_bits_list[i];

            if(xm->shaders.KeyExist(ssb))
            {
                sea[i].Enable();
//                sea[i].fn_edit->setText(ToQString(xm->shaders[ssb]->short_name));
            }
            else
                sea[i].Disable();
        }
    }
}

bool MaterialEditorWidget::OnCloseRequested()
{
    return(true);
}
