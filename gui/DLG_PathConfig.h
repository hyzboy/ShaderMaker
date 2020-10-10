#pragma once

#include<QDialog>
#include<QGridLayout>
#include<QLineEdit>

class DLGPathConfig:public QDialog
{
    Q_OBJECT

private:

    QGridLayout *grid_layout;

    QLineEdit *path_edit[3];

    void CreateGroup(QWidget *,int,const QString &);

public:

    DLGPathConfig();
    ~DLGPathConfig()=default;
};//
