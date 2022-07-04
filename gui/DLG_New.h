#pragma once

#include<QDialog>
#include<QLabel>
#include<QLineEdit>

class DLGNew:public QDialog
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

    DLGNew(const QString &,const QString &);
    virtual ~DLGNew()=default;

    virtual void OnOKClicked()=0;
};//class DLGNew:public QDialog
