#pragma once

#include<QDialog>
#include<QLabel>
#include<QLineEdit>

class DLGNameEdit:public QDialog
{
    Q_OBJECT

private:

    QString type_name;

    QLineEdit *name_edit;
    QLabel *hint_label;

private:

    bool result;

    void OnCancelClicked();

public:

    DLGNameEdit(const QString &,const QString &);
    virtual ~DLGNameEdit()=default;

    virtual void OnOKClicked()=0;
};//class DLGNameEdit:public QDialog
