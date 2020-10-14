#pragma once

#include<QWidget>
#include<QTreeWidget>
#include<QPushButton>
#include<QLabel>
#include<QHBoxLayout>
#include"GLSLTextEdit.h"
#include<hgl/type/String.h>
#include"InfoOutput.h"

class TPMaterialLibrary:public QWidget
{
    Q_OBJECT

private:

    QTreeWidget *file_tree_widget;
    QTreeWidgetItem *current_item;

    void UpdateFileTree();
    void OnFileChanged(QTreeWidgetItem *,int);

    QTabWidget *editor_tab_widget;

    void OnTabCloseRequested(int index);

public:

    TPMaterialLibrary();
    ~TPMaterialLibrary()=default;
};//