#pragma once

#include<hgl/type/String.h>
#include<QDialog>
#include<QGridLayout>
#include<QLineEdit>

class DLGConfig:public QDialog
{
    Q_OBJECT

private:

    QGridLayout *grid_layout;

    QLineEdit *path_edit[3];

    void CreateGroup(QWidget *,int,const QString &,const hgl::OSString &);

    QPushButton *ui_font_button;
    QPushButton *code_font_button;

    void UpdateFontButtonText(QPushButton *,const QString &,const QFont &);
    QPushButton *CreateFontConfig(QWidget *,QLayout *,const QString &,const QFont &,void (DLGConfig::*)());

private:

    void OnUIFontClicked();
    void OnCodeFontClicked();

    void OnOKClicked();
    void OnCancelClicked();

public:

    DLGConfig();
    ~DLGConfig()=default;
};//
