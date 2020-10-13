#pragma once

#include<QWidget>
#include<QTreeWidget>
#include<QPushButton>
#include<QLabel>
#include<QHBoxLayout>
#include"GLSLTextEdit.h"
#include<hgl/type/String.h>
#include"InfoOutput.h"

#define ML_TYPE_MATERIAL   "Material"
#define ML_TYPE_VERTEX     "Vertex"
#define ML_TYPE_GEOMETRY   "Geometry"
#define ML_TYPE_FRAGMENT   "Fragment"

#define ML_COLUMN_NAME     0
#define ML_COLUMN_TYPE     1
#define ML_COLUMN_FILENAME 2

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