#pragma once

#include<QDialog>

class DLGAbout:public QDialog
{
    Q_OBJECT

private:

    QWidget *CreateIconAndNameLayout(QWidget *parent);

    QWidget *CreateTabpage(QWidget *parent);

private:

    void OnClose();

public:

    DLGAbout();
    ~DLGAbout()=default;
};//
