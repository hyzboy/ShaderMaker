#pragma once

#include<hgl/type/String.h>
#include<QDialog>
#include<QGridLayout>
#include<QLineEdit>

class DLGPathConfig:public QDialog
{
    Q_OBJECT

private:

    QGridLayout *grid_layout;

    QLineEdit *path_edit[3];

    void CreateGroup(QWidget *,int,const QString &,const hgl::OSString &);

private:

    void OnOKClicked();
    void OnCancelClicked();

public:

    DLGPathConfig();
    ~DLGPathConfig()=default;
};//
