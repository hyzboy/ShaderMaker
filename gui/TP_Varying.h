#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include"GLSLTextEdit.h"

class TPVarying:public QWidget
{
    Q_OBJECT

private:

    void OnVaryingClicked(QListWidgetItem *item);

private:

    QTabWidget *editor_tab_widget;

    void OnTabCloseRequested(int index);

public:

    TPVarying();
    ~TPVarying()=default;
};
