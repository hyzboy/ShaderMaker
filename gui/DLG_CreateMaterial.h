#pragma once

#include<vulkan/vulkan.h>
#include<QDialog>
#include<QCheckBox>
#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>

struct ShaderUseWidget
{    
    QLabel *        label;
    QLineEdit *     edit;
    QPushButton *   browser;

    void OnBrowserClicked();

public:

    ShaderUseWidget(const QString &name,QWidget *parent,QGridLayout *layout,int row,bool check);

    void SetChecked(bool);
};

class DLGCreateMaterial:public QDialog
{
    Q_OBJECT

private:
    
    QCheckBox *use_tess_shader,
              *use_geom_shader;

    ShaderUseWidget *vertex,*tess_control,*tess_eval,*geometry,*fragment;

    void OnTessStateChanged(int state);
    void OnGeomStateChanged(int state);

    void OnVertexBrowserClicked();
    void OnTessControlBrowserClicked();
    void OnTessEvalBrowserClicked();
    void OnGeometryBrowserClicked();
    void OnFragmentBrowserClicked();

private:    
        
    void OnOKClicked();
    void OnCancelClicked();

public:

    DLGCreateMaterial();
    ~DLGCreateMaterial()=default;
};//class DLGCreateMaterial:public QDialog
